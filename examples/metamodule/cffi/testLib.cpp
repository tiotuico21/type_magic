#include <dlfcn.h>
#include <iostream>

int main()
{
    void *handle = dlopen("./my_dynamic_library.so", RTLD_NOW);
    std::cout << "handle: " << handle << std::endl;

    void *fn_ptr = dlsym(handle, "trait_a");
    void *fn_ptr_b = dlsym(handle, "trait_b");
    void *constructor = dlsym(handle, "construct");

    std::cout << fn_ptr << std::endl;

    void (*trait_a)(void *, int, bool) = (void (*)(void *, int, bool))fn_ptr;
    float (*trait_b)(void *, double) = (float (*)(void *, double))fn_ptr_b;
    void *(*construct)() = (void *(*)())constructor;

    void *context = construct();
    // void *context;
    trait_a(context, 2, true);

    float val = trait_b(context, 4);

    std::cout << "return val: " << val << std::endl;
}