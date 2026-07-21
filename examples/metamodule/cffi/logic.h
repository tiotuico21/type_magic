#include <typeinfo>
#include "../../../include/include.h"

#include <chrono>
#include <thread>

#include <functional>

#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define STRINGIFY_HELPER(x) #x
#define LINE_STRING STRINGIFY(__LINE__)
#ifdef COLOR_ASSERTS
#define ASSERT_TEXT(TEXT) "\n\n\e[33m\e[1m" __FILE__ ":" LINE_STRING " \e[0m\e[33m" TEXT "\e[0m\n"
#else
#define ASSERT_TEXT(TEXT) "\n\n" __FILE__ ":" LINE_STRING TEXT "\n"
#endif

// value

#include <unordered_map>
#include <string>
#include <stdexcept>

#include <unordered_map>
#include <string>
#include <stdexcept>

std::vector<std::string> extern_func_headers = {
    "def construct():",
    "def destruct(ptr):",
    "def alloc_bytes(ptr, arg1):",
    "def free_bytes(ptr, arg1):",
    "def trait_a_fn(ptr, arg1, arg2):",
    "def ret_str(ptr):",
    "def trait_b_fn(ptr, arg1):",
};

std::vector<std::string> extern_linker_headers = {};

std::vector<std::string> extern_func_param = {"", "ptr", "ptr, arg1", "ptr, arg1", "ptr, arg1, arg2", "ptr", "ptr, arg1"};
std::string static cppToNumbaType(std::string cppType)
{
    static const std::unordered_map<std::string, std::string> typeMap = {
        {"bool", "types.boolean"},

        {"char", "types.int8"},
        {"signed char", "types.int8"},
        {"unsigned char", "types.uint8"},

        {"short", "types.int16"},
        {"unsigned short", "types.uint16"},

        {"int", "types.int32"},
        {"unsigned", "types.uint32"},
        {"unsigned int", "types.uint32"},

        {"long", "types.int64"},
        {"unsigned long", "types.uint64"},

        {"long long", "types.int64"},
        {"unsigned long long", "types.uint64"},

        {"int8_t", "types.int8"},
        {"uint8_t", "types.uint8"},
        {"int16_t", "types.int16"},
        {"uint16_t", "types.uint16"},
        {"int32_t", "types.int32"},
        {"uint32_t", "types.uint32"},
        {"int64_t", "types.int64"},
        {"uint64_t", "types.uint64"},

        {"float", "types.float32"},
        {"double", "types.float64"},
        {"std::string",
         "CPointer(types.int8)"},
        {"std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
         "CPointer(types.int8)"},
        {"void", "types.void"}};

    // Remove trailing whitespace
    while (!cppType.empty() && std::isspace(cppType.back()))
        cppType.pop_back();

    int pointerDepth = 0;

    // Count trailing '*'
    while (!cppType.empty() && cppType.back() == '*')
    {
        ++pointerDepth;
        cppType.pop_back();

        while (!cppType.empty() && std::isspace(cppType.back()))
            cppType.pop_back();
    }

    // Special case: void*
    if (cppType == "void" && pointerDepth == 1)
        return "types.voidptr";

    auto it = typeMap.find(cppType);

    if (it == typeMap.end())
        throw std::runtime_error("Unknown C++ type: " + cppType);

    std::string result = it->second;

    for (int i = 0; i < pointerDepth; ++i)
    {
        result = "types.CPointer(" + result + ")";
    }

    return result;
}

template <auto... THING>
struct Fn
{
};

// type
template <typename RES, typename CLASS, typename... ARGS, RES (CLASS::*X)(ARGS...)>
struct Fn<X>
{
    static constexpr auto value = X;
    typedef RES Result;
    typedef CLASS Class;
    typedef container::TypeArray<ARGS...> Args;
    typedef RES (CLASS::*type)(ARGS...);

    static auto call(CLASS *self, ARGS... args)
    {
        return (self->*X)(args...);
    }
};

template <typename TYPE>
struct IsFn
{
    static constexpr bool value = false;
};

template <auto ARG>
struct IsFn<Fn<ARG>>
{
    static constexpr bool value = true;
};

template <typename TRAIT>
struct FFIEntry
{
};

struct FFIGen
{
};

/*
static_assert(
                !LossyCombineType::duplicate_key,
                ASSERT_TEXT(
                    "ERROR: Combine operation resulted in duplicate keys. "
                    "If duplicates should be coalesced, use TypeMap's LossyCombine operation instead."));
*/

template <typename... ENTRIES>
struct StaticTable
{
    typedef container::TypeMap<ENTRIES...> EntriesTypeMap;
    static_assert(
        EntriesTypeMap::template FilterItems<container::util::Negate<IsFn>::template Template>::type::ITEM_COUNT == 0,
        ASSERT_TEXT("ERROR: No bindings that have items with the appropriate Fn format"));

    template <typename TRAIT, typename... ARGS>
    static auto call(ARGS... args)
    {
        // are we going into the typemap namespace to then get item at but
        // then how would it know which type map
        typedef typename EntriesTypeMap::template ItemAt<TRAIT>::type FnPtr;
        // return func_ptr::call(func_ptr::Args)
        return FnPtr::call(args...);
    }
};

struct TraitA
{
    //  AConstruct{};
    struct AFn
    {
    };
    struct AGetStr
    {
    };
    typedef StaticTable<
        // container::Binding<AConstruct, (this) * ()>,
        container::Binding<AFn, void *(int, bool)>,
        container::Binding<AGetStr, std::string *()>>
        STable;
};

struct TraitB
{
    struct BFn
    {
    };
    typedef StaticTable<
        container::Binding<BFn, float *(double)>>
        STable;
};

struct Alloc
{
    struct AllocFun
    {
    };
    struct FreeFun
    {
    };
    typedef StaticTable<
        container::Binding<AllocFun, void *(size_t)>,
        container::Binding<FreeFun, void(void *)>>
        STable;
};

template <typename CONTEXT>
struct AllocImpl
{
    size_t mult;
    AllocImpl()
    {
        mult = 2;
    }
    void *alloc_fun_impl(size_t size)
    {
        return malloc(size * mult);
    }
    void free_fun_impl(void *ptr)
    {
        std::cout << "Freeing " << ptr << std::endl;
        return free(ptr);
    }

    typedef StaticTable<
        container::Binding<
            Alloc::AllocFun,
            Fn<
                &AllocImpl<CONTEXT>::alloc_fun_impl>>,
        container::Binding<
            Alloc::FreeFun,
            Fn<
                &AllocImpl<CONTEXT>::free_fun_impl>>>
        STable;
};

using AllocModule = context::SimpleModule<
    Meta<AllocImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<Alloc, FFIEntry<Alloc>>>;
// sp the bindings are making the alloc with fn to a type so to speak which is a class and we are saying
// the template will have 0 to more arguments of types of any given auto

template <typename CONTEXT>
struct BImpl
{
    int y;
    BImpl()
    {
        y = 10;
    }
    float fn(double x)
    {
        std::cout << "entered trait b and will multiplying input by 3" << std::endl;
        float returnVal = x * y;
        return returnVal;
    }
    typedef StaticTable<
        container::Binding<
            TraitB::BFn, Fn<&BImpl<CONTEXT>::fn>>>
        STable;
};

using BModule = context::SimpleModule<
    Meta<BImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<TraitB, FFIEntry<TraitB>>>;

template <typename CONTEXT>
struct AImpl
{
    std::string member;
    AImpl()
    {
        member = "test member";
    }
    std::string retStr()
    {
        return member;
    }
    void fn(int x, bool y)
    {
        std::cout << "member: " << member << std::endl;
        if (y)
        {
            std::cout << "hello there trait_a was true so here is your number: " << x << std::endl;
        }
        else
        {
            std::cout << "hello there trait a was false so i will not show your number" << std::endl;
        }
    }
    typedef StaticTable<
        container::Binding<
            TraitA::AFn, Fn<&AImpl<CONTEXT>::fn>>,
        container::Binding<TraitA::AGetStr, Fn<&AImpl<CONTEXT>::retStr>>>
        STable;

    /*
      typedef StaticTable<
        container::Binding<
            Alloc::AllocFun,
            Fn<
                &AllocImpl<CONTEXT>::alloc_fun_impl>>,
        container::Binding<
            Alloc::FreeFun,
            Fn<
                &AllocImpl<CONTEXT>::free_fun_impl>>>
        STable;
    */
};

using AModule = context::SimpleModule<
    Meta<AImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<TraitA, FFIEntry<TraitA>>>;

template <typename Class, typename... Types>
struct TypeList
{
};

template <typename T>
struct PureFnEq;

template <typename RES, typename CLASS, typename... ARGS>
struct PureFnEq<RES (CLASS::*)(ARGS...)>
{
    typedef RES type(CLASS *, ARGS...);
    // do i make a new template struct js fir the type list
    // why do we havw to iterate thru the class *
    // what kinda type is res

    // am i even declaring a type here if all i want is a Type Array hold the arguments which are params
    typedef container::TypeArray<CLASS *, ARGS...> Args;
    typedef RES Result;
};

// how does it work when you add two type def to this struct bc wwhen you pass in a param what do u do to return the one u want

template <typename CONTEXT>
struct FFIGenImpl
{
    // FFIGen(){}
    void genffi(std::string client_logic_header_file)
    {
        std::fstream header_file;
        std::fstream cpp_file;
        std::fstream python_file;
        header_file.open("cffi.h", std::ios::trunc | std::ios::out);
        cpp_file.open("cffi.cpp", std::ios::trunc | std::ios::out);
        python_file.open("test_ffi.py", std::ios::trunc | std::ios::out);

        python_file << "import re\n"
                    << "import sys\n"
                    << "import numba\n"
                    << "import inspect\n"
                    << "import subprocess\n"
                    << "\n"
                    << "from llvmlite import binding\n"
                    << "\n"
                    << "binding.load_library_permanently(\"./my_dynamic_library.so\")\n";

        /*
        import re
import sys
import numba
import inspect
import subprocess

from llvmlite import binding

binding.load_library_permanently("../my_dynamic_library.so")
        */
        addFunctionHeaders(header_file, python_file, client_logic_header_file);
        addFunctionBody(cpp_file, python_file);
        header_file.close();
        cpp_file.close();

        for (size_t i = 0; i < extern_func_headers.size(); ++i)
        {
            python_file << "@numba.njit\n";
            python_file << extern_func_headers[i] << "\n\t"
                        << "return " << extern_linker_headers[i] << "(" << extern_func_param[i] << ")\n\n";
        }
        python_file.close();

        // why fPIC smthn about address reolacation
        char *args[] = {
            (char *)"g++",
            (char *)"-std=c++20",
            (char *)"-shared",
            (char *)"-fPIC",
            (char *)"cffi.cpp",
            (char *)"-o",
            (char *)"my_dynamic_library.so",
            nullptr};

        execvp("g++", args);
    }

    template <typename T>
    void addFunctionGenRecurse(std::fstream &gen_file, std::fstream &python_file, bool isCpp)
    {
        if constexpr (std::is_same<T, container::TypeSet<>>::value)
        {
            std::cout << "all empty" << std::endl;
            return;
        }
        // why did i have to guard this with an else
        else
        {
            typedef typename T::MapType::HeadItemType CurrFFISpec;
            typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::cout << generated_func_name << std::endl;
            std::string trait_name = container::repr::type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCase(trait_name);

            // function sig
            typedef As<CurrTrait, CONTEXT> sig_component;

            // type map of functions we can then iterate through to do whats below
            // goin to need new func mangle names
            // or we can go into the static table type map
            // 178 calls a recursive function using specialized structs
            // takes a static table
            // expose the first argument of the template binding for func name
            // what is the template fn though
            // already know the Trait
            // typedef decltype(&sig_component::fn) method_pointer_sig;

            std::string func_sig = container::repr::type_name<CONTEXT>(); //
            if (isCpp)
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(true, trait_name, gen_file, python_file);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, isCpp);
            }
            else
            {

                //    std::cout << "func sig -> " << func_sig << std::endl;

                // func param list
                // get the args list

                // iterate through the classes to because each can have their own param list

                // should prob put stuff in a helper method

                /*
                // i want to input the method pointer sig and reach in to get the args type def to make a type list i can iterate over
                typedef typename PureFnEq<method_pointer_sig>::Args method_args_list;
                typedef typename method_args_list::template PopFront<>::type inner_args_list;
                typedef typename inner_args_list::template PushFront<CONTEXT *>::type outter_args_list;

                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                // std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                //   std::cout << "param_list:" << param_list << std::endl;


                std::string resultType = container::repr::type_name<typename PureFnEq<method_pointer_sig>::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

                std::cout << "writing to the cffi.h" << std::endl;
                gen_file << "extern \"C\" "
                         << resultType
                         << " "
                         << generated_func_name
                         << "("
                         << header
                         << ")"
                         << ";"
                         << std::endl;
                */
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(false, trait_name, gen_file, python_file);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, false);
            }
        }
    }

    template <typename... T>
    struct ReadEveryFunction;

    template <typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<TAIL...>>
    {

        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, int func_index = 0)
        {
            std::cout << "i am going into the base case of readComponentFunctions" << std::endl;
            return;
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, int func_index = 0)
        {
            std::string mangle_func_name = typeid(KEY).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            std::string reg_str_func_name = container::repr::type_name<KEY>();

            std::cout << "________________________REGULAR FUNC" << reg_str_func_name << std::endl;

            if (isCpp)
            {
                // how will item know it is fn struct
                typedef typename ITEM::Args method_args_list;

                //  typedef typename method_args_list::template PopFront<>::type inner_args_list;
                // typedef typename inner_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                // its blank
                std::cout << "arg list: " << arg_list << std::endl;
                std::string resultType = container::repr::type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

                /*
                extern_trait_a_fn = numba.types.ExternalFunction(
    "_TYPEMAGICN6TraitA3AFnE",
    numba.core.typing.signature(
        numba.types.void,
        numba.types.voidptr,
        numba.types.int32,
        numba.types.bool #boolean?????
    )
)
                */
                std::string extern_function_return_type = "numba." + cppToNumbaType(resultType);
                std::string extern_function_param_list = ParamListToString<outter_args_list>::makeString(0, true, true);
                python_file << "extern_" << toSnakeCase(reg_str_func_name) << " = numba.types.ExternalFunction(\n\t\""
                            << typemagic_mangle_name
                            << "\",\n\tnumba.core.typing.signature(\n\t\t"
                            << extern_function_return_type
                            << ",\n\t\t"
                            << extern_function_param_list
                            << "\n\t)\n)\n\n";
                extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name));
                gen_file << "extern \"C\" "
                         << resultType << " " << typemagic_mangle_name << "(" << header << ")"
                         << "{"
                         << std::endl
                         << "\t"
                         << container::repr::type_name<CONTEXT>()
                         << "* ptr = ("
                         << container::repr::type_name<CONTEXT>()
                         << "*) arg0;"
                         << std::endl
                         << "\treturn As<"
                         << traitName
                         << ", "
                         << container::repr::type_name<CONTEXT>()
                         << ">::STable::template call<typename "
                         << reg_str_func_name
                         << ">(&(as<"
                         << traitName;
                if (arg_list.size() == 0)
                {
                    gen_file << ">(*ptr)));";
                }
                else
                {
                    gen_file << ">(*ptr)), "
                             << arg_list
                             << ");";
                }
                gen_file << std::endl
                         << "}"
                         << std::endl;
                ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, func_index + 1);
            }
            else
            {
                typedef typename ITEM::Args method_args_list;
                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;

                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);

                std::string resultType = container::repr::type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

                std::cout << "writing to the cffi.h" << std::endl;
                gen_file << "extern \"C\" "
                         << resultType
                         << " "
                         << typemagic_mangle_name
                         << "("
                         << header
                         << ")"
                         << ";"
                         << std::endl;
                std::cout << "finished writing to the cffi.h with the new functrion gen function" << std::endl;
                // ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType)
                ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, func_index + 1);
            }
        }
    };

    void addConstructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp)
    {

        if (extern_linker_headers.size() == 0 || extern_linker_headers[0] != "extern_construct")
        {
            python_file << "extern_contruct = numba.types.ExternalFunction(\n\t\"construct\",\n\tnumba.core.typing.signature(\n\t\tnumba.types.voidptr\n\t)\n)\n\n";
            extern_linker_headers.push_back("extern_construct");
        }

        gen_file << "extern \"C\" void* construct()";

        // container::repr::type_name<CONTEXT>()
        if (isCpp)
        {
            gen_file << "{"
                     << std::endl
                     << "\treturn (void*) new "
                     << container::repr::type_name<CONTEXT>()
                     << ";"
                     << std::endl
                     << "}"
                     << std::endl;
        }
        else
        {
            gen_file << ";"
                     << std::endl;
        }
    }

    void addDestructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp)
    {

        if (extern_linker_headers.size() == 0 || extern_linker_headers[1] != "extern_destruct")
        {
            python_file << "extern_destruct = numba.types.ExternalFunction(\n\t\"destruct\",\n\tnumba.core.typing.signature(\n\t\tnumba.types.void,\n\t\tnumba.types.voidptr\n\t)\n)\n\n";
            extern_linker_headers.push_back("extern_destruct");
        }
        gen_file << "extern \"C\" void destructor(void* ptr)";
        if (isCpp)
        {
            gen_file << "{"
                     << std::endl
                     << container::repr::type_name<CONTEXT>()
                     << "*ptr_to_delete = ("
                     << container::repr::type_name<CONTEXT>()
                     << "*) ptr;"
                     << "\treturn delete ptr_to_delete;"
                     << "}";
        }
        else
        {
            gen_file << ";"
                     << std::endl;
        }
    }

    static std::string toSnakeCase(std::string &trait_name)
    {
        std::string snake_case_trait_name;

        for (size_t i = 0; i < trait_name.size(); i++)
        {
            if (std::isupper(static_cast<unsigned char>(trait_name[i])) || static_cast<unsigned char>(trait_name[i]) == ':')
            {
                if (i != 0)
                {
                    snake_case_trait_name += '_';
                }
                if (static_cast<unsigned char>(trait_name[i]) != ':')
                {
                    snake_case_trait_name += std::tolower(static_cast<unsigned char>(trait_name[i]));
                }
            }
            else
            {
                snake_case_trait_name += trait_name[i];
            }
        }
        return snake_case_trait_name;
    }

    template <typename T>
    struct ParamListToString;

    // template <typename... ARGS>
    template <typename... TAIL>
    struct ParamListToString<container::TypeArray<TAIL...>>
    {
        static std::string makeString(int index, bool includeType, bool forPython = false)
        {
            std::cout << "i am going into the base case" << std::endl;
            return "";
        }
    };

    template <typename HEAD, typename... TAIL>
    struct ParamListToString<container::TypeArray<HEAD, TAIL...>>
    {
        static std::string makeString(int index, bool includeType, bool forPython = false)
        {
            std::cout << "i am going into the recursive case" << std::endl;
            std::string str_head_type = container::repr::type_name<HEAD>();

            std::string str_types_from_tail = ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType, forPython);

            std::cout << "index: " << index << "-> " << str_head_type << std::endl;
            std::string total_param_list = "";
            if (forPython)
            {
                std::string current =
                    (index == 0)
                        ? "numba." + cppToNumbaType("void*")
                        : "numba." + cppToNumbaType(str_head_type);

                if (str_types_from_tail.empty())
                    return current;

                return current + ", " + str_types_from_tail;
            }
            if (includeType)
            {
                if (index == 0)
                {
                    total_param_list += "void*";
                }
                else
                {
                    total_param_list += container::repr::type_name<HEAD>();
                }
            }
            if (container::TypeArray<TAIL...>::MapType::ITEM_COUNT == 0)
            {
                if (includeType)
                {
                    total_param_list += " arg" + std::to_string(index);
                }
                else
                {
                    total_param_list += "arg" + std::to_string(index);
                }
            }
            else
            {
                if (includeType)
                {
                    total_param_list += " arg" + std::to_string(index) + ", " + str_types_from_tail;
                }
                else
                {
                    total_param_list += "arg" + std::to_string(index) + ", " + str_types_from_tail;
                }
            }
            // std::cout << "toal param inside tostring recursive: " << total_param_list << std::endl;
            return total_param_list;
        }
    };

    int findSpace(std::string func_sig)
    {
        bool entered = false;
        int depth = 0;
        for (int i = func_sig.length(); i >= 0; i--)
        {

            if (func_sig[i] == ')' && !entered)
            {
                depth = 1;
                entered = true;
            }
            else if (func_sig[i] == '(' && entered)
            {
                depth--;
                if (depth == 0)
                {
                    return i - 1;
                }
            }

            else if (func_sig[i] == ')' && entered)
            {
                depth++;
            }
        }
        return -1;
    }

    // idk if we need his as its a template , i feel like i need to wire smthn so that we js filter for FFI

    void addFunctionHeaders(std::fstream &header_file, std::fstream &python_file, std::string client_header)
    {
        /*
        #include <thread>
        #include "../../../include/include.h"
        #include <functional>
        #include <iostream>
        #include <fstream>
        */
        header_file << "#include <thread>"
                    << std::endl
                    << "#include \"../../../include/include.h\""
                    << std::endl
                    << "#include <functional>"
                    << std::endl
                    << "#include <iostream>"
                    << std::endl
                    << "#include <fstream>"
                    << std::endl
                    << "#include \"" << client_header << "\""
                    << std::endl;

        addConstructor(header_file, python_file, false);
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFISet; // get every FFI specialization
        addFunctionGenRecurse<FFISet>(header_file, python_file, false);
        addDestructor(header_file, python_file, false);
    }

    void addFunctionBody(std::fstream &cpp_file, std::fstream &python_file)
    {
        std::cout << "adding function body" << std::endl;
        cpp_file << "#include \"cffi.h\""
                 << std::endl
                 << std::endl;
        addConstructor(cpp_file, python_file, true);
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        addFunctionGenRecurse<FFICppSet>(cpp_file, python_file, true);
        addDestructor(cpp_file, python_file, true);
    }

    /*
    void makeBody()
    {
    }


    extern "C" float b(double x)
    {
        return via<TraitB>(this).fn(x);
    }
    extern "C" float a(int x, int y)
    {
        return via<TraitA>(this).fn(x, y);
    }
        */
};

using FFIGenModule = context::SimpleModule<
    Meta<FFIGenImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<FFIGen>>;
