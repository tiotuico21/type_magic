#include <dlfcn.h>
#include <iostream>

int main()
{
    void *handle = dlopen("./my_dynamic_library.so", RTLD_NOW);
    std::cout << "handle: " << handle << std::endl;

    void *fn_ptr = dlsym(handle, "_TYPEMAGICN6TraitA3AFnE");
    char *err = dlerror();
    if (err)
    {
        std::cerr << "TraitA: " << err << '\n';
    }

    void *fn_ret_str_ptr = dlsym(handle, "_TYPEMAGICN6TraitA7AGetStrE");
    err = dlerror();
    if (err)
    {
        std::cerr << "RetStr: " << err << '\n';
    }
    void *fn_ptr_b = dlsym(handle, "_TYPEMAGICN6TraitB3BFnE");
    err = dlerror();
    if (err)
    {
        std::cerr << "TraitB: " << err << '\n';
    }
    void *constructor = dlsym(handle, "construct");
    err = dlerror();
    if (err)
    {
        std::cerr << "construct: " << err << '\n';
    }

    void *destructor = dlsym(handle, "destructor");
    err = dlerror();
    if (err)
    {
        std::cerr << "construct: " << err << '\n';
    }
    void *fn_alloc_ptr = dlsym(handle, "_TYPEMAGICN5Alloc8AllocFunE");
    err = dlerror();
    if (err)
    {
        std::cerr << "Alloc " << err << '\n';
    }

    void *fn_free_ptr = dlsym(handle, "_TYPEMAGICN5Alloc7FreeFunE");
    err = dlerror();
    if (err)
    {
        std::cerr << "Free " << err << '\n';
    }

    std::cout << fn_ptr << std::endl;
    std::cout << fn_ret_str_ptr << std::endl;
    std::cout << fn_ptr_b << std::endl;
    std::cout << constructor << std::endl;
    std::cout << destructor << std::endl;
    std::cout << fn_alloc_ptr << std::endl;
    std::cout << fn_free_ptr << std::endl;

    void (*trait_a)(void *, int, bool) = (void (*)(void *, int, bool))fn_ptr;
    float (*trait_b)(void *, double) = (float (*)(void *, double))fn_ptr_b;
    std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> (*ret_str)(void *) = (std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> (*)(void *))fn_ret_str_ptr;
    void *(*construct)() = (void *(*)())constructor;
    void (*destruct)(void *) = (void (*)(void *))destructor;
    void *(*fn_alloc)(void *, unsigned long) = (void *(*)(void *, unsigned long))fn_alloc_ptr;
    void (*fn_free)(void *, void *) = (void (*)(void *, void *))fn_free_ptr;

    void *context = construct();
    // void *context;
    trait_a(context, 2, true);

    float val = trait_b(context, 4);

    void *alloc_test = fn_alloc(context, 10);

    char *p = static_cast<char *>(alloc_test);

    std::cout << "STRING RET: " << ret_str(context) << std::endl;

    for (int i = 0; i < 20; ++i)
    {
        p[i] = static_cast<char>(i);
    }

    for (int i = 0; i < 20; ++i)
    {
        std::cout << (int)p[i] << " ";
    }
    std::cout << std::endl;

    fn_free(context, alloc_test);

    std::cout << "all good" << std::endl;

    std::cout << "return val: " << val << std::endl;

    destruct(context);
}