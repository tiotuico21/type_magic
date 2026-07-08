#include <dlfcn.h>
#include <iostream>

int main()
{
    void *handle = dlopen("./my_dynamic_library.so", RTLD_NOW);
    std::cout << "handle: " << handle << std::endl;

    void *fn_ptr = dlsym(handle, "_TYPEMAGICNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEETraitA");
    void *fn_ptr_b = dlsym(handle, "_TYPEMAGICNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEETraitB");
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