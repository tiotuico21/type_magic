
import numba
from numba import types
from numba import cfunc
from llvmlite import binding
import inspect 
#from type_magic import CONTEXT
import subprocess

import re

class CONTEXT:
    pass

class MiscFuncForNumba:
    def make_cpp_to_numba_fun_str():
        str_to_numba = r'''
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
    '''
        return str_to_numba

class FFIGenCodeGetter:
    @staticmethod
    def make_genffi_function():
        func_genffi_str = r'''
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
        

       
    }
            
    '''
        return func_genffi_str
    
    @staticmethod
    def make_add_func_gen_recurse():
        func_gen_recurse_str = r'''
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

            typedef As<CurrTrait, CONTEXT> sig_component;

            std::string func_sig = container::repr::type_name<CONTEXT>(); 
            if (isCpp)
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(true, trait_name, gen_file, python_file);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, isCpp);
            }
            else
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(false, trait_name, gen_file, python_file);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, false);
            }
        }
    }
    
    '''
        return func_gen_recurse_str
    
    @staticmethod
    def make_read_every_function():
        readEveryFunction_str = r'''
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
                typedef typename ITEM::Args method_args_list;

                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                std::cout << "arg list: " << arg_list << std::endl;
                std::string resultType = container::repr::type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            
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

    '''
        return readEveryFunction_str
    
    @staticmethod
    def make_constructor_str():
        constructor_str = r'''
    void addConstructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp)
    {

        if (extern_linker_headers.size() == 0 || extern_linker_headers[0] != "extern_construct")
        {
            python_file << "extern_construct = numba.types.ExternalFunction(\n\t\"construct\",\n\tnumba.core.typing.signature(\n\t\tnumba.types.voidptr\n\t)\n)\n\n";
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
        
    '''
        return constructor_str

    def make_destructor_str():
        destructor_str = r'''
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
    '''
        return destructor_str
    
    @staticmethod
    def to_snake_case_func_str():
        str_to_snake_func = r'''
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
    '''
        return str_to_snake_func
    
    @staticmethod
    def param_to_string_str():
        str_make_string = r'''
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
    '''
        return str_make_string


    @staticmethod
    def find_space_str():
        str_find_space = r'''
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
    '''
        return str_find_space
    
    @staticmethod
    def ffi_header_body_helper_str():
        helper_str = r'''
     void addFunctionHeaders(std::fstream &header_file, std::fstream &python_file, std::string client_header)
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
    '''
        return helper_str
    
    @staticmethod
    def make_ffi_module_str():
        str_ffi_module = r'''
using FFIGenModule = context::SimpleModule<
    Meta<FFIGenImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<FFIGen>>;
    '''
        return str_ffi_module
        
    
              
    @staticmethod
    def make_full_ffi_impl_struct():
        str_struct_header = "template <typename CONTEXT>\nstruct FFIGenImpl{\n"
        str_genffi_func = FFIGenCodeGetter.make_genffi_function()
        str_func_gen_recurse = FFIGenCodeGetter.make_add_func_gen_recurse()
        str_read_every_func = FFIGenCodeGetter.make_read_every_function()
        str_constructor = FFIGenCodeGetter.make_constructor_str()
        str_destructor = FFIGenCodeGetter.make_destructor_str()
        str_snake_case_func = FFIGenCodeGetter.to_snake_case_func_str()
        str_param_to_string = FFIGenCodeGetter.param_to_string_str()
        str_find_space = FFIGenCodeGetter.find_space_str()
        str_ffi_header_body_generator = FFIGenCodeGetter.ffi_header_body_helper_str()

        str_footer = "\n};\n\n"
        str_module = FFIGenCodeGetter.make_ffi_module_str()

        str_recurse_logic = f"{str_genffi_func}{str_func_gen_recurse}{str_read_every_func}"
        str_init_and_clean = f"{str_constructor}{str_destructor}"
        str_helper_funcs = f"{str_snake_case_func}{str_param_to_string}{str_find_space}{str_ffi_header_body_generator}"
        return f"{str_struct_header}{str_recurse_logic}{str_init_and_clean}{str_helper_funcs}{str_footer}{str_module}"

    
    @staticmethod
    def make_main_cpp_file(fn_list):
        str_run_method = '''
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "logic.h"

using RootModule = context::ModuleBundle<AddOneModule, AddItModule, FFIGenModule>;

template <typename CTX>
void run()
{
	std::cout << "Entered run" << std::endl;

	if constexpr (CTX::Info::SATISFIED)
	{
		std::cout << "Context satisfied" << std::endl;

		CTX ctx{};

		std::cout << "Context created" << std::endl;

		as<FFIGen>(ctx).genffi("logic.h");

		std::cout << "FFI generated" << std::endl;
	}
	else
	{
		CTX ctx{};
		std::cout << "Context not satisfied" << std::endl;
		std::cout << as<context::ContextInfo>(ctx).error_string();
	}
}
    '''
        str_main_method = FFIGenCodeGetter.make_main_func_in_main_cpp(fn_list)
        str_main_cpp = str_run_method + str_main_method
        return str_main_cpp

    @staticmethod
    def make_main_func_in_main_cpp(fn_list):
        str_main_method = '''
    int main()
    {
        typedef typename context::CreateContextType<
            RootModule,
    '''
        trait_list = []
        for fn in fn_list:
            trait_list.append(UtilStrings.to_pascal_case(fn.__name__))

        for fn in fn_list:
            trait_list.append(UtilStrings.make_ffi_wrapper_str(fn))

        trait_list.append("FFIGen")
        
        str_main_method += "\t\tcontainer::TypeSet<"
        str_main_method += ", ".join(trait_list) + ">,"
        str_main_method += "\n\t\t\tMeta<context::EagerSolve>>::type Ctx;"

        str_main_method += '''
        run<Ctx>();
        return 0;
    }
    '''
        return str_main_method







class UtilStrings:
    @staticmethod
    def to_pascal_case(name: str) -> str:
        return "".join(word.capitalize() for word in name.split("_"))
    
    @staticmethod
    def make_ffi_wrapper_str(fn):
        str_cpp_ffi = "FFIEntry<" + (UtilStrings.to_pascal_case(fn.__name__)) + ">"
        return str_cpp_ffi
    
    '''
std::vector<std::string> extern_func_headers = {
    "def construct():",
    "def destruct(ptr):",
    "def alloc_bytes(ptr, arg1):",
    "def free_bytes(ptr, arg1):",
    "def trait_a_fn(ptr, arg1, arg2):",
    "def ret_str(ptr):",
    "def trait_b_fn(ptr, arg1):",
};
'''
    @staticmethod
    def make_Apy_signature_str_list(fn_list):
        signature_list = ApyGenerator.make_func_signature_Apy_list(fn_list)

        str_vector_definition = "std::vector<std::string> extern_func_headers = {"

        str_sig_list = "\n\t"
        str_sig_list += ",\n\t".join(f'"{sig}"' for sig in signature_list)
        str_sig_list += "\n};"

        return str_vector_definition + str_sig_list

    @staticmethod
    def make_Apy_arguments_str_list(fn_list):
        param_list_array = ApyGenerator.make_array_of_all_param_lists(fn_list)

        str_vector_definition = "std::vector<std::string> extern_func_param = {"
        str_param_list = "\n\t"
        str_param_list += ",\n\t".join(f'"{params}"' for params in param_list_array)
        str_param_list += "\n};"

        return str_vector_definition + str_param_list
    
    def make_empty_linker_header_array_str():
        return r'''std::vector<std::string> extern_linker_headers = {};'''
    
    @staticmethod
    def make_module_for_fn_str(fn_name):
        str_fn_module = ""
        str_fn_module += "using " + UtilStrings.to_pascal_case(fn_name) + "Module = context::SimpleModule<\n\t"
        str_fn_module += "Meta<Impl" + UtilStrings.to_pascal_case(fn_name) + ">,\n\t"
        str_fn_module += "context::RequirementSet<>,\n\t"
        str_fn_module += "context::ImplementationSet<" + UtilStrings.to_pascal_case(fn_name) + ", FFIEntry<" + UtilStrings.to_pascal_case(fn_name) + ">>\n"
        str_fn_module += ">;\n\n"
        return str_fn_module
    
    @staticmethod
    def make_logic_h_include():
        str_logic_h_include = ""
        str_logic_h_include += "#include <typeinfo>\n"
        str_logic_h_include += "#include \"../../../../include/include.h\"\n\n"
        str_logic_h_include += "#include <chrono>\n"
        str_logic_h_include += "#include <thread>\n\n"
        str_logic_h_include += "#include <functional>\n\n"
        str_logic_h_include += "#define STRINGIFY(x) STRINGIFY_HELPER(x)\n"
        str_logic_h_include += "#define STRINGIFY_HELPER(x) #x\n"
        str_logic_h_include += "#define LINE_STRING STRINGIFY(__LINE__)\n"
        str_logic_h_include += "#ifdef COLOR_ASSERTS\n"
        str_logic_h_include += "#define ASSERT_TEXT(TEXT) \"\\n\\n\\e[33m\\e[1m\" __FILE__ \":\" LINE_STRING \" \\e[0m\\e[33m\" TEXT \"\\e[0m\\n\"\n"
        str_logic_h_include += "#else\n"
        str_logic_h_include += "#define ASSERT_TEXT(TEXT) \"\\n\\n\" __FILE__ \":\" LINE_STRING TEXT \"\\n\"\n"
        str_logic_h_include += "#endif\n\n\n"
        return str_logic_h_include
    
    @staticmethod
    def make_fn_struct_in_logic_h():
        str_fn_struct_in_logic_h = ""
        str_fn_struct_in_logic_h += "template <auto... THING>\nstruct Fn\n{\n};\n\n"
        str_fn_struct_in_logic_h += "template <typename RES, typename CLASS, typename... ARGS, RES (CLASS::*X)(ARGS...)>\n"
        str_fn_struct_in_logic_h += "struct Fn<X>\n{\n\tstatic constexpr auto value = X;\n\ttypedef RES Result;\n"
        str_fn_struct_in_logic_h += "\ttypedef CLASS Class;\n"
        str_fn_struct_in_logic_h += "\ttypedef container::TypeArray<ARGS...> Args;\n\ttypedef RES (CLASS::*type)(ARGS...);\n\n"
        str_fn_struct_in_logic_h += "\tstatic auto call(CLASS *self, ARGS... args)\n\t{\n"
        str_fn_struct_in_logic_h += "\t\treturn (self->*X)(args...);\n\t}\n};\n\n"
        str_fn_struct_in_logic_h += "template <typename TYPE>\nstruct IsFn\n{\n"
        str_fn_struct_in_logic_h += "\tstatic constexpr bool value = false;\n"
        str_fn_struct_in_logic_h += "};\n\n"
        str_fn_struct_in_logic_h += "template <auto ARG>\n"
        str_fn_struct_in_logic_h += "struct IsFn<Fn<ARG>>\n"
        str_fn_struct_in_logic_h += "{\n"
        str_fn_struct_in_logic_h += "\tstatic constexpr bool value = true;\n"
        str_fn_struct_in_logic_h += "};\n\n"
        return str_fn_struct_in_logic_h

    @staticmethod
    def make_ffi_trait_struct_in_logic_h():
        str_ffi_entry_trait_struct_in_logic_h = ""
        str_ffi_entry_trait_struct_in_logic_h += "template <typename TRAIT>\n"
        str_ffi_entry_trait_struct_in_logic_h += "struct FFIEntry\n"
        str_ffi_entry_trait_struct_in_logic_h += "{\n"
        str_ffi_entry_trait_struct_in_logic_h += "};\n\n"
        str_ffi_entry_trait_struct_in_logic_h += "struct FFIGen\n"
        str_ffi_entry_trait_struct_in_logic_h += "{\n"
        str_ffi_entry_trait_struct_in_logic_h += "};\n\n"
        return str_ffi_entry_trait_struct_in_logic_h

    @staticmethod
    def make_static_table_struct_in_logic_h():
        str_static_table_struct_in_logic_h = ""
        str_static_table_struct_in_logic_h += "template <typename... ENTRIES>\n"
        str_static_table_struct_in_logic_h += "struct StaticTable\n"
        str_static_table_struct_in_logic_h += "{\n"
        str_static_table_struct_in_logic_h += "\ttypedef container::TypeMap<ENTRIES...> EntriesTypeMap;\n"
        str_static_table_struct_in_logic_h += "\tstatic_assert(\n"
        str_static_table_struct_in_logic_h += "\t\tEntriesTypeMap::template FilterItems<container::util::Negate<IsFn>::template Template>::type::ITEM_COUNT == 0,\n"
        str_static_table_struct_in_logic_h += "\t\tASSERT_TEXT(\"ERROR: No bindings that have items with the appropriate Fn format\"));\n\n"
        str_static_table_struct_in_logic_h += "\ttemplate <typename TRAIT, typename... ARGS>\n"
        str_static_table_struct_in_logic_h += "\tstatic auto call(ARGS... args)\n"
        str_static_table_struct_in_logic_h += "\t{\n"
        str_static_table_struct_in_logic_h += "\t\ttypedef typename EntriesTypeMap::template ItemAt<TRAIT>::type FnPtr;\n"
        str_static_table_struct_in_logic_h += "\t\treturn FnPtr::call(args...);\n"
        str_static_table_struct_in_logic_h += "\t}\n"
        str_static_table_struct_in_logic_h += "};\n\n"
        return str_static_table_struct_in_logic_h

    @staticmethod
    def make_type_list_str_in_logic_h():
        str_type_list_def = "template <typename Class, typename... Types>\nstruct TypeList\n{\n};\n"
        return str_type_list_def

    @staticmethod
    def make_pure_fn_eq_struct_in_logic_h():
        str_pure_fn_eq_struct_in_logic_h = ""
        str_pure_fn_eq_struct_in_logic_h += "template <typename T>\n"
        str_pure_fn_eq_struct_in_logic_h += "struct PureFnEq;\n\n"
        str_pure_fn_eq_struct_in_logic_h += "template <typename RES, typename CLASS, typename... ARGS>\n"
        str_pure_fn_eq_struct_in_logic_h += "struct PureFnEq<RES (CLASS::*)(ARGS...)>\n"
        str_pure_fn_eq_struct_in_logic_h += "{\n"
        str_pure_fn_eq_struct_in_logic_h += "\ttypedef RES type(CLASS *, ARGS...);\n"
        str_pure_fn_eq_struct_in_logic_h += "\ttypedef container::TypeArray<CLASS *, ARGS...> Args;\n"
        str_pure_fn_eq_struct_in_logic_h += "\ttypedef RES Result;\n"
        str_pure_fn_eq_struct_in_logic_h += "};\n\n"
        return str_pure_fn_eq_struct_in_logic_h
    
    def make_STABLE_for_trait_component(fn):
        str_STABLE_def = r'''
typedef StaticTable<
        container::Binding<
'''
        str_STABLE_def += UtilStrings.to_pascal_case(fn.__name__) + "::CallFn, "
        str_STABLE_def += "Fn<&Impl" + UtilStrings.to_pascal_case(fn.__name__) 
        str_STABLE_def += "<CONTEXT>::call>>>\n\t\t"
        str_STABLE_def += "STable;\n"

        return str_STABLE_def


'''
_______________________________
    typedef StaticTable<
        container::Binding<
            TraitA::AFn, Fn<&AImpl<CONTEXT>::fn>>,
        container::Binding<TraitA::AGetStr, Fn<&AImpl<CONTEXT>::retStr>>>
        STable;
'''

    

class AnnotationGetter:
    @staticmethod
    def get_str_return_type(fn) -> str:
        sig = inspect.signature(fn)
        return_type = sig.return_annotation
        return return_type.__name__

    @staticmethod
    def get_str_param_list(fn) -> str:
        sig = inspect.signature(fn)
        params = []
        for name, param in sig.parameters.items():
            param_type = param.annotation.__name__
            if (param_type == "CONTEXT"):
                param_type = "void*"
                params.append(f"{param_type} {name}")
                continue
            if (param_type == "str"):
                param_type = "std::string"
                params.append(f"{param_type} {name}")
            else:
                params.append(f"{param_type} {name}")

        print(f"Param list: {params}")
        return ", ".join(params)
    
    import inspect

    def get_python_param_types(fn):
        types = []

        for param in inspect.signature(fn).parameters.values():
            annotation = param.annotation

            if isinstance(annotation, str):
                types.append(annotation)
            else:
                if (annotation.__name__ == "CONTEXT"):
                    types.append("voidptr")
                    continue
                types.append(annotation.__name__)

        return types

    @staticmethod
    def get_param_type_list(fn):
        sig = inspect.signature(fn)
        param_types = []
        for name, param in sig.parameters.items():
            if (param.annotation.__name__ == "CONTEXT"):
                param_types.append("void*")
                continue
            if (param.annotation.__name__ == "str"):
                param_types.append("std::string")
            else:
                param_types.append(f"{param.annotation.__name__}")
        return param_types

    @staticmethod
    def pythonToNumbaType(pyType):
        typeMap = {
            "bool": types.boolean,

            "char": types.int8,
            "signed char": types.int8,
            "unsigned char": types.uint8,

            "short": types.int16,
            "unsigned short": types.uint16,

            "int": types.int64,
            "unsigned": types.uint64,
            "unsigned int": types.uint64,

            "long": types.int64,
            "unsigned long": types.uint64,

            "long long": types.int64,
            "unsigned long long": types.uint64,

            "float": types.float32,
            "double": types.float64,

            "str": types.CPointer(types.int8),
            "bytes": types.CPointer(types.uint8),

            "void": types.void,
            "voidptr": types.voidptr,
        }

        if not isinstance(pyType, str):
            pyType = pyType.__name__

        pyType = pyType.strip()

        pointerDepth = 0

        while pyType.endswith("*"):
            pointerDepth += 1
            pyType = pyType[:-1].strip()

        if pyType == "void" and pointerDepth == 1:
            return types.voidptr

        if pyType not in typeMap:
            raise RuntimeError(f"Unknown Python type: {pyType}")

        result = typeMap[pyType]

        for _ in range(pointerDepth):
            result = types.CPointer(result)

        return result
            
    @staticmethod
    def make_numba_signature(fn, param_list):
        sig = inspect.signature(fn)

        ret = AnnotationGetter.pythonToNumbaType(sig.return_annotation)
        args = [
            AnnotationGetter.pythonToNumbaType(p)
            for p in param_list
        ]
        return ret(*args)

class ApyGenerator:
    @staticmethod
    def make_func_param_list_Apy(fn):
        sig = inspect.signature(fn)
        param_list = []
        param_list.append("ptr")
        paramAmount = 0
        for name, param in sig.parameters.items():
            if (paramAmount == 0):
                paramAmount += 1
                continue
            param_list.append(f"arg{paramAmount}")
            paramAmount += 1
        print(param_list)
        return param_list
    
    @staticmethod
    def make_array_of_all_param_lists(fn_list):
        str_array_of_param_lists = []
        str_array_of_param_lists.append("")
        str_array_of_param_lists.append("ptr")
        for fn in fn_list:
            str_array_of_param_lists.append("(" + ", ".join(ApyGenerator.make_func_param_list_Apy(fn)) + ")")
        print("AAAA")
        print(str_array_of_param_lists)
        return str_array_of_param_lists
        
    @staticmethod
    def make_func_signature_Apy(fn, fn_name):
        str_extern_linker = ""
        str_extern_linker += "def " + fn_name + "("
        param_list = ApyGenerator.make_func_param_list_Apy(fn)
        str_extern_linker += ", ".join(param_list) + "):"
        return str_extern_linker

    @staticmethod
    def make_func_signature_Apy_list(fn_list):
        str_extern_linker_list = []
        str_extern_linker_list.append("def construct():")
        str_extern_linker_list.append("def destruct(ptr):")
        for fn in fn_list:
            str_extern_linker_list.append(ApyGenerator.make_func_signature_Apy(fn, fn.__name__))
        return str_extern_linker_list



'''
def add_one(ctx: CONTEXT, arg1: int, arg2: str, arg3: bool) -> int:
    return arg1 + 1

'''
def add_one(ctx: CONTEXT, arg1: int, arg2: bool) -> int:
    return arg1 + 1

def add_it(ctx: CONTEXT) -> int:
    return 1




'''
std::vector<std::string> extern_func_headers = {
    "def construct():",
    "def destruct(ptr):",
    "def alloc_bytes(ptr, arg1):",
    "def free_bytes(ptr, arg1):",
    "def trait_a_fn(ptr, arg1, arg2):",
    "def ret_str(ptr):",
    "def trait_b_fn(ptr, arg1):",
};
'''





def make_func_component_str(fn, fn_name):
    sig = inspect.signature(fn)
    str_cpp_extern_func = "extern \"C\" int " + fn_name + "(" + AnnotationGetter.get_str_return_type(fn) + "* ret"
    param_list = AnnotationGetter.get_str_param_list(fn)
    if len(AnnotationGetter.get_param_type_list(fn)) == 1:
        str_cpp_extern_func += ", void* ctx);"
    else:
        str_cpp_extern_func += ", " + param_list + ");"
    str_cpp_component = "template <typename CONTEXT>\nstruct Impl" + UtilStrings.to_pascal_case(fn_name) + "{\n\t" + AnnotationGetter.get_str_return_type(fn) + " call("
    
    print(f"length: {len(param_list)}")
    if len(AnnotationGetter.get_param_type_list(fn)) == 1:
        str_cpp_component += AnnotationGetter.get_str_param_list(fn) + "){\n\t\t" + AnnotationGetter.get_str_return_type(fn) + " result;\n\t\t"
        str_cpp_component += fn_name + "(&result, (CONTEXT*)this);\n\t\t"
        str_cpp_component += "return result;\n\t}\n\t" 
        str_cpp_component += UtilStrings.make_STABLE_for_trait_component(fn) 
        str_cpp_component += "\n};"
    else:
        body_arg_list = []
        index = 0
        for name, param in sig.parameters.items():
            if (index == 0):
                index += 1
                continue
            body_arg_list.append(f"arg{index}")
            index += 1
        str_cpp_component += AnnotationGetter.get_str_param_list(fn) + "){\n\t\t" + AnnotationGetter.get_str_return_type(fn) + " result;\n\t\t"
        str_cpp_component += fn_name + "(&result, (CONTEXT*)this," + ", ".join(body_arg_list) + ");\n\t\t"
        str_cpp_component += "return result;\n\t}\n\t"
        str_cpp_component += UtilStrings.make_STABLE_for_trait_component(fn)
        str_cpp_component += "\n};"
    full_cpp_code = str_cpp_extern_func + "\n" + str_cpp_component
    return full_cpp_code

def make_func_trait_str(fn, fn_name):
    sig_= inspect.signature(fn)
    str_cpp_trait = ""
    str_cpp_trait += "struct " + UtilStrings.to_pascal_case(fn_name) + "{\n\t"
    str_cpp_trait += "struct CallFn{};\n\t"
    str_cpp_trait += "typedef StaticTable<" + "\n\t\tcontainer::Binding<CallFn, int* (" + AnnotationGetter.get_str_return_type(fn) + "*, "
    param_type_list = AnnotationGetter.get_param_type_list(fn)
    if not param_type_list:
        str_cpp_trait += ")>>\n\t"
    else:
        str_cpp_trait += ", ".join(param_type_list) + ")>>\n\t"
    str_cpp_trait += "STable;\n" + "};"

    return str_cpp_trait

'''
param_list = AnnotationGetter.get_python_param_types(add_one)

signature = AnnotationGetter.make_numba_signature(add_one, param_list)
print(signature)

numba_fn = numba.njit(add_one)

numba_fn.compile(signature)

print(numba_fn.signatures)

llvm_ir = numba_fn.inspect_llvm(numba_fn.signatures[0])

# Write it to a .ll file
with open("add.ll", "w") as f:
    f.write(llvm_ir)

'''



'''
with open("add_one.ll") as f:
    llvm = f.read()


#@_ZN8__main__7add_oneB2v1B38c8tJTIcFKzyF2ILShI4CrgQElQb6HczSBAA_3dE7void_2axb
pattern = re.compile(r"_ZN8__main__[0-9]([A-Za-z_][A-Za-z0-9_]*)[^(]+")
match = re.search(pattern, llvm)

group = match.group()


llvm = llvm[:match.start()] + "add_one" + llvm[match.end():]

with open("add_one.ll", "w") as f:
    f.write(llvm)
'''


def modify_llvm_func_name(fn):
    with open(f"{fn.__name__}.ll") as f:
        llvm_contents = f.read()
    #dont use ^ and $ bc thinks it has to end with that
    #search instead of match
    pattern = re.compile(r"_ZN8__main__[0-9]([A-Za-z_][A-Za-z0-9_]*)[^(]+")
    match = re.search(pattern, llvm_contents)

    new_llvm_contents = llvm_contents[:match.start()] + fn.__name__ + llvm_contents[match.end():]

    with open(f"{fn.__name__}.ll", "w") as f:
        f.write(new_llvm_contents)
    
def make_cpp_dict(fn_list):
    cpp_dict = {}
    cpp_dict["RequirementSet"] = {}
    cpp_dict["RequiredTraits"] = []
    cpp_dict["LL_Files"] = []
    for item in fn_list:
        cpp_dict["RequirementSet"][item.__name__] = [make_func_trait_str(item, item.__name__), make_func_component_str(item, item.__name__)]
        cpp_dict["RequiredTraits"].append(UtilStrings.to_pascal_case(item.__name__))
        param_list = AnnotationGetter.get_python_param_types(item)
        signature = AnnotationGetter.make_numba_signature(item, param_list)
        numba_fn = numba.njit(item)
        numba_fn.compile(signature)
        llvm_ir = numba_fn.inspect_llvm(numba_fn.signatures[0])
        with open(f"{item.__name__}.ll", "w") as f:
            f.write(llvm_ir)

        cpp_dict["LL_Files"].append(f"{item.__name__}.ll")
        modify_llvm_func_name(item)

    print(cpp_dict)
    print(cpp_dict["LL_Files"])

    return cpp_dict


def make_logic_h(fn_list):
    include_str = UtilStrings.make_logic_h_include()
    empty_linker_array_str = UtilStrings.make_empty_linker_header_array_str()
    fn_struct_str = UtilStrings.make_fn_struct_in_logic_h()
    ffi_trait_str = UtilStrings.make_ffi_trait_struct_in_logic_h()
    static_table_str = UtilStrings.make_static_table_struct_in_logic_h()
    type_list_def_str = UtilStrings.make_type_list_str_in_logic_h()
    pure_fn_eq_str = UtilStrings.make_pure_fn_eq_struct_in_logic_h()
    gen_ffi__struct_str = FFIGenCodeGetter.make_full_ffi_impl_struct()
    apy_sig_list = UtilStrings.make_Apy_signature_str_list(fn_list)
    apy_param_list = UtilStrings.make_Apy_arguments_str_list(fn_list)
    cpp_to_numba_str = MiscFuncForNumba.make_cpp_to_numba_fun_str()

    cpp_dict = make_cpp_dict(fn_list)

    with open ("logic.h", "w") as f:
        f.write(f"{include_str}\n{apy_sig_list}\n{apy_param_list}\n{cpp_to_numba_str}\n{empty_linker_array_str}\n{fn_struct_str}\n{ffi_trait_str}\n{static_table_str}\n")
        #f.write("std::string ")
        for name, code_body in cpp_dict["RequirementSet"].items():
            f.write(code_body[0])
            f.write("\n")
        f.write("\n")
        for name, code_body in cpp_dict["RequirementSet"].items():
            f.write(f"{code_body[1]}\n{UtilStrings.make_module_for_fn_str(name)}\n")
        f.write("\n")
        f.write(f"{type_list_def_str}\n{pure_fn_eq_str}\n{gen_ffi__struct_str}")

    
def make_main_cpp(fn_list):
    str_main_cpp = FFIGenCodeGetter.make_main_cpp_file(fn_list)

    with open ("main.cpp", "w") as f:
        f.write(f"{str_main_cpp}")


def compile_and_run(fn_list, main_file_name):
    make_logic_h(fn_list)
    make_main_cpp(fn_list)


    fn_trait_dict = make_cpp_dict(fn_list)

    ll_list = fn_trait_dict["LL_Files"]
    obj_list = [ll.replace(".ll", ".o") for ll in ll_list]
    result = subprocess.run(
        [
            "g++",
            "-std=c++20",
            main_file_name,
            "-o",
            "main" 
        ],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print("Compilation failed:")
        print(result.stderr)
        return
    
    print("compilation complete")

    result = subprocess.run(
        ["./main"],
        #=True,
        text=True
    )

    result = subprocess.run(
        [
            "clang++",
            "-std=c++20",
            "-shared",
            "-fPIC",
            "cffi.cpp",
            *ll_list,
            "-o",
            "my_dynamic_library.so"
        ]
    )


    if result.returncode != 0:
        print("Progeam exited with an error")
        print(result.stderr)
        return
    


#compile_and_run([add_one, add_it], "main.cpp")

#make_cpp_dict([add_it, add_one])

@numba.njit(numba.double(numba.double, numba.double))
def func(x, y):
    # A somewhat trivial example
    return x + y

func(1.0, 2.0)
sig = func.signatures[0]


#print(sig)
llvm_ir = func.inspect_llvm(sig)


#print(llvm_ir)
with open("f.ll", "w") as fp:
    fp.write(llvm_ir)


'''
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
'''