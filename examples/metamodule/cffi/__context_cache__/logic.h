#include <typeinfo>
#include "../../../../include/include.h"

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



#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>


template <typename TYPE>
static std::string query(){
    return __PRETTY_FUNCTION__;
}

template <typename TYPE>
static std::string get_type_name(){
    std::string voidPRETTY = query<void>();
    std::string ourPRETTY = query<TYPE>();

    int typeIndex = voidPRETTY.find("void");

    int difference = ourPRETTY.length() - voidPRETTY.length();

    int totalLength = difference + 4;

    return ourPRETTY.substr(typeIndex, totalLength);

}


std::vector<std::string> extern_func_headers = {
	"def construct():",
	"def destruct(ptr):",
	"def add_one(ptr, arg1, arg2, arg3):",
	"def add_it(ptr):",
	"def sub_args(ptr, arg1, arg2, arg3):",
	"def is_true(ptr, arg1):"
};
std::vector<std::string> extern_func_param = {
	"()",
	"(ptr)",
	"(ptr, arg1, arg2, arg3)",
	"(ptr)",
	"(ptr, arg1, arg2, arg3)",
	"(ptr, arg1)",
	"(retptr, ptr, arg1, arg2, arg3)",
	"(retptr, ptr)",
	"(retptr, ptr, arg1, arg2, arg3)",
	"(retptr, ptr, arg1)"
};

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


std::vector<std::string> extern_linker_headers = {};
template <auto... THING>
struct Fn
{
};

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
		typedef typename EntriesTypeMap::template ItemAt<TRAIT>::type FnPtr;
		return FnPtr::call(args...);
	}
};



template <typename T>
struct Print{
    struct PrintFn{};

    typedef StaticTable<
        container::Binding<PrintFn, void(T)>>
        STable;
};



struct AddOne{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, float(void*, float, bool, float)>>
	STable;
};
struct AddIt{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int(void*)>>
	STable;
};
struct SubArgs{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, float(void*, float, bool, float)>>
	STable;
};
struct IsTrue{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, bool(void*, bool)>>
	STable;
};

template <typename T>
struct PrintImplMeta{
    template <typename CONTEXT>
    struct PrintImpl{
        void my_print(T item){
            std::cout << "Print: " << item << std::endl;
        }
        typedef StaticTable<
            container::Binding<
                typename Print<T>::PrintFn,
                Fn<&PrintImplMeta::template PrintImpl<CONTEXT>::my_print>
            >
        >
        STable;
    };

    typedef context::SimpleModule <
        Meta<PrintImpl>,
        context::RequirementSet<>,
        context::ImplementationSet<Print<T>, FFIEntry<Print<T>>>
    > Module;
};

template <typename T>
struct PrintImplFFIMeta {
    typedef context::EmptyModule Module;
};

template <typename T>
struct PrintImplFFIMeta <Print<T>> { 
    typedef context::SimpleModule <
            Meta<PrintImplMeta<T>::template PrintImpl>,
            context::RequirementSet<>,
            context::ImplementationSet<Print<T>, FFIEntry<Print<T>>>
        > Module;
};




typedef context::ModuleBundle<
    context::MetaModule <
        Print,
        PrintImplMeta
    >,
    context::MetaModule <
        FFIEntry,
        PrintImplFFIMeta
    >
> PrintModule;




extern "C" float add_one(void* ctx, float arg1, bool arg2, float arg3);
template <typename CONTEXT>
struct ImplAddOne{
	float call(float arg1, bool arg2, float arg3){
		float result;
		return add_one((CONTEXT*)this,arg1, arg2, arg3);
	}
	
typedef StaticTable<
        container::Binding<
AddOne::CallFn, Fn<&ImplAddOne<CONTEXT>::call>>>
		STable;

};
using AddOneModule = context::SimpleModule<
	Meta<ImplAddOne>,
	context::RequirementSet<>,
	context::ImplementationSet<AddOne, FFIEntry<AddOne>>
>;


extern "C" int add_it(void* ctx);
template <typename CONTEXT>
struct ImplAddIt{
	int call(){
		int result;
		return add_it((CONTEXT*)this);
	}
	
typedef StaticTable<
        container::Binding<
AddIt::CallFn, Fn<&ImplAddIt<CONTEXT>::call>>>
		STable;

};
using AddItModule = context::SimpleModule<
	Meta<ImplAddIt>,
	context::RequirementSet<>,
	context::ImplementationSet<AddIt, FFIEntry<AddIt>>
>;


extern "C" float sub_args(void* ctx, float arg1, bool arg2, float arg3);
template <typename CONTEXT>
struct ImplSubArgs{
	float call(float arg1, bool arg2, float arg3){
		float result;
		return sub_args((CONTEXT*)this,arg1, arg2, arg3);
	}
	
typedef StaticTable<
        container::Binding<
SubArgs::CallFn, Fn<&ImplSubArgs<CONTEXT>::call>>>
		STable;

};
using SubArgsModule = context::SimpleModule<
	Meta<ImplSubArgs>,
	context::RequirementSet<>,
	context::ImplementationSet<SubArgs, FFIEntry<SubArgs>>
>;


extern "C" bool is_true(void* ctx, bool arg1);
template <typename CONTEXT>
struct ImplIsTrue{
	bool call(bool arg1){
		bool result;
		return is_true((CONTEXT*)this,arg1);
	}
	
typedef StaticTable<
        container::Binding<
IsTrue::CallFn, Fn<&ImplIsTrue<CONTEXT>::call>>>
		STable;

};
using IsTrueModule = context::SimpleModule<
	Meta<ImplIsTrue>,
	context::RequirementSet<>,
	context::ImplementationSet<IsTrue, FFIEntry<IsTrue>>
>;



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
	typedef container::TypeArray<CLASS *, ARGS...> Args;
	typedef RES Result;
};


template <typename CONTEXT>
struct FFIGenImpl{

    void genffi(std::string client_logic_header_file, bool is_for_CPU)
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

        addFunctionHeaders(header_file, python_file, client_logic_header_file, is_for_CPU);
        addFunctionBody(cpp_file, python_file, is_for_CPU);
        header_file.close();
        cpp_file.close();

        for (size_t i = 0; i < extern_func_headers.size(); ++i)
        {
            python_file << "@numba.njit(cache=False)\n";
            python_file << extern_func_headers[i] << "\n\t"
                        << "return " << extern_linker_headers[i] << extern_func_param[i] << "\n\n";
        }

        /*
        def print_int(ptr, arg1):
	        return extern__TYPEMAGICN5PrintIiE7PrintFnE(ptr, arg1)
        */
        python_file << "@numba.njit(cache=False)\n";
        python_file << "def print_int(ptr, arg1):\n\treturn extern__TYPEMAGICN5PrintIiE7PrintFnE(ptr, arg1)\n\n";

        std::cout << python_file.is_open() << '\n';
        python_file.close();

        // why fPIC smthn about address reolacation
        

       
    }


    template <typename T>
    void addFunctionGenRecurse(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {
        if constexpr (std::is_same<T, container::TypeSet<>>::value)
        {
            return;
        }
        // why did i have to guard this with an else
        else
        {
            typedef typename T::MapType::HeadItemType CurrFFISpec;
            typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::string trait_name = get_type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCase(trait_name);

            typedef As<CurrTrait, CONTEXT> sig_component;

            std::string func_sig = get_type_name<CONTEXT>(); 
            if (isCpp)
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(true, trait_name, gen_file, python_file, is_for_CPU);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, isCpp, is_for_CPU);
            }
            else
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(false, trait_name, gen_file, python_file, is_for_CPU);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, false, is_for_CPU);
            }
        }
    }


    template <typename... T>
    struct ReadEveryFunction;

    template <typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<TAIL...>>
    {

        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {
            return;
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {
            std::string mangle_func_name = typeid(KEY).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string reg_str_func_name = get_type_name<KEY>();

            std::cout << "________________________REGULAR FUNC" << reg_str_func_name << std::endl;

            if (isCpp)
            {
                typedef typename ITEM::Args method_args_list;

                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            
                std::string extern_function_return_type = "numba." + cppToNumbaType(resultType);
                std::string extern_function_param_list = ParamListToString<outter_args_list>::makeString(0, true, true);


                if (is_for_CPU){
                    //python_file << "extern_" << toSnakeCase(reg_str_func_name) << " = numba.types.ExternalFunction(\n\t\""
                    python_file << "extern_" << typemagic_mangle_name << " = numba.types.ExternalFunction(\n\t\""
                                                << typemagic_mangle_name
                                                << "\",\n\tnumba.core.typing.signature(\n\t\t"
                                                << extern_function_return_type + ", \n\t\t"
                                                << extern_function_param_list
                                                << "\n\t)\n)\n\n";
                    //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name));
                    extern_linker_headers.push_back("extern_" + typemagic_mangle_name);
                    
                    gen_file << "extern \"C\" "
                             << resultType << " " << typemagic_mangle_name << "(" << header << ")"
                             << "{"
                             << std::endl
                             << "\t"
                             << get_type_name<CONTEXT>()
                             << "* ptr = ("
                             << get_type_name<CONTEXT>()
                             << "*) arg0;"
                             << std::endl
                             << "\treturn As<"
                             << traitName
                             << ", "
                             << get_type_name<CONTEXT>()
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
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{
                    python_file << "extern_" << toSnakeCase(reg_str_func_name) << "_gpu = cuda.declare_device(\n\t\""
                                             << typemagic_mangle_name + "_gpu" << "\", \n\tnumba.core.typing.signature("
                                             << extern_function_return_type
                                             << "(" << extern_function_param_list << ")))\n\n";
                    //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name) + "_gpu");
                    extern_linker_headers.push_back("extern_" + typemagic_mangle_name);
                    gen_file << "extern \"C\" "
                             << "int" << " " << typemagic_mangle_name + "_gpu" << "(" << resultType + "* retptr, " + header << ")"
                             << "{"
                             << std::endl
                             << "\t"
                             << get_type_name<CONTEXT>()
                             << "* ptr = ("
                             << get_type_name<CONTEXT>()
                             << "*) arg0;"
                             << std::endl
                             << "\t*retptr = As<"
                             << traitName
                             << ", "
                             << get_type_name<CONTEXT>()
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
                                << "\treturn 1;"
                                << "\n}"
                                << std::endl;
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
            }
            else
            {
                typedef typename ITEM::Args method_args_list;
                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;

                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

                if (is_for_CPU){
                    gen_file << "extern \"C\" "
                             << resultType
                             << " "
                             << typemagic_mangle_name
                             << "("
                             << header
                             << ")"
                             << ";"
                             << std::endl;
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{
                   gen_file << "extern \"C\" "
                            << "int"
                            << " "
                            << typemagic_mangle_name + "_gpu"
                            << "("
                            << resultType + "* retptr, " + header
                            << ")"
                            << ";"
                            << std::endl;
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                
                // ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType)
            }
        }
    };


    void addConstructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if (extern_linker_headers.size() == 0 || extern_linker_headers[0] != "extern_construct")
        {
            if (is_for_CPU){
                python_file << "extern_construct = numba.types.ExternalFunction(\n\t\"construct\",\n\tnumba.core.typing.signature(numba.types.voidptr)\n)\n\n";
                extern_linker_headers.push_back("extern_construct");               
            }
            else{
                 python_file << "extern_construct_gpu = cuda.declare_device(\n\t\""
                             << "construct_gpu\", \n\tnumba.core.typing.signature("
                             << "numba.types.voidptr"
                             << "()))\n\n";
                 extern_linker_headers.push_back("extern_construct_gpu");
            }
        }
 

        // container::repr::type_name<CONTEXT>()
        if (isCpp)
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void* construct()";
                gen_file << "{"
                         << std::endl 
                         << "\tvoid* myPtr = (void*) new "
                         << get_type_name<CONTEXT>()
                         << ";"
                         << std::endl
                         << "\tstd::cout << \"Constructor Ptr:\" << myPtr << std::endl;"
                         << std::endl
                         << "return myPtr;"
                         << "\n}"
                         << std::endl;
            }
            else{
                gen_file << "extern \"C\" int construct_gpu(void* retptr)";
                gen_file << "{"
                         << std::endl
                         << "\tretptr = (void*) new "
                         << get_type_name<CONTEXT>()
                         << ";"
                         << std::endl
                         << "\treturn 1;"
                         << "\n}"
                         << std::endl;
            }
        }
        else
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void* construct();" << std::endl;
            }
            else{
                gen_file << "extern \"C\" int construct_gpu(void* retptr);" << std::endl;
            }
        }
    }
     

    void addDestructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if (extern_linker_headers.size() <= 1 || extern_linker_headers[1] != "extern_destruct")
        {
            if (is_for_CPU){
                python_file << "extern_destruct = numba.types.ExternalFunction(\n\t\"destructor\",\n\tnumba.core.typing.signature(\n\t\tnumba.types.voidptr, numba.types.voidptr\n\t)\n)\n\n";
                extern_linker_headers.push_back("extern_destruct");
            }
            else{
                python_file << "extern_destruct_gpu = cuda.declare_device(\n\t\""
                            << "destructor_gpu\", \n\tnumba.core.typing.signature("
                            << "numba.types.void"
                            << "(numba.types.voidptr)))\n\n";
                extern_linker_headers.push_back("extern_destruct_gpu");
            }
        }

        if (isCpp)
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void destructor(void* ptr)";
                gen_file << "{"
                         << std::endl
                         << get_type_name<CONTEXT>()
                         << "*ptr_to_delete = ("
                         << get_type_name<CONTEXT>()
                         << "*) ptr;"
                         << "\n\treturn delete ptr_to_delete;"
                                     
                         << "\n}\n\n";
            }
            else{
                //gen_file << "extern \"C\" __device__\n";
                gen_file << "extern \"C\" int destructor_gpu(void* retptr, void* ptr)";
                gen_file << "{"
                         << std::endl
                         << get_type_name<CONTEXT>()
                         << "*ptr_to_delete = ("
                         << get_type_name<CONTEXT>()
                         << "*) ptr;"
                         << "\n\tdelete ptr_to_delete;"
                         << "\n\treturn 1;"
                         << "\n}\n";
            }
        }
        else
        {
            if (is_for_CPU){
               gen_file << "extern \"C\" void destructor(void* ptr);" << std::endl;
            }
            else{
               gen_file << "extern \"C\" int destructor_gpu(void* retptr, void* ptr);" << std::endl;
            } 
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
            return "";
        }
    };

    template <typename HEAD, typename... TAIL>
    struct ParamListToString<container::TypeArray<HEAD, TAIL...>>
    {
        static std::string makeString(int index, bool includeType, bool forPython = false)
        {
            // std::cout << "i am going into the recursive case" << std::endl;
            std::string str_head_type = get_type_name<HEAD>();

            std::string str_types_from_tail = ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType, forPython);

            // std::cout << "index: " << index << "-> " << str_head_type << std::endl;
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
                    total_param_list += get_type_name<HEAD>();
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


     void addFunctionHeaders(std::fstream &header_file, std::fstream &python_file, std::string client_header, bool is_for_CPU)
    {
        header_file << "#include <thread>"
                    << std::endl
                    << "#include \"../../../../include/include.h\""
                    << std::endl
                    << "#include <functional>"
                    << std::endl
                    << "#include <iostream>"
                    << std::endl
                    << "#include <fstream>"
                    << std::endl
                    << "#include \"" << client_header << "\""
                    << std::endl;

        addConstructor(header_file, python_file, false, is_for_CPU);
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFISet; // get every FFI specialization
        addFunctionGenRecurse<FFISet>(header_file, python_file, false, is_for_CPU);
        addDestructor(header_file, python_file, false, is_for_CPU);
    }

    void addFunctionBody(std::fstream &cpp_file, std::fstream &python_file, bool is_for_CPU)
    {
        //std::cout << "adding function body" << std::endl;
        cpp_file << "#include \"cffi.h\""
                 << std::endl
                 << std::endl;
        addConstructor(cpp_file, python_file, true, is_for_CPU);
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        addFunctionGenRecurse<FFICppSet>(cpp_file, python_file, true, is_for_CPU);
        addDestructor(cpp_file, python_file, true, is_for_CPU);
    }


};


using FFIGenModule = context::SimpleModule<
    Meta<FFIGenImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<FFIGen>>;
    