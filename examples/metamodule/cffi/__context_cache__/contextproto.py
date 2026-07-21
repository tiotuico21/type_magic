
import numba
from numba import types
from numba import cfunc
from llvmlite import binding
import inspect 
#from type_magic import CONTEXT

class CONTEXT:
    pass


def add_one(ctx: CONTEXT, arg1: int, arg2: str, arg3: bool) -> int:
    return arg1 + 1

def add_it(ctx: CONTEXT) -> int:
    return 1
'''
@numba.njit
def alloc_bytes(ptr, arg1):
    return extern_Alloc_alloc___alloc_fun(ptr, arg1)
'''
def make_func_extern_linker(fn, fn_name):
    str_extern_linker = ""
    str_extern_linker += "@numba.njit\n" + "def " + fn_name + "("
    sig = inspect.signature(fn)
    param_list = []
    paramAmount = 0
    for name, param in sig.parameters.items():
        if (paramAmount == 0):
            param_list.append("ptr")
        else:
            param_list.append(f"arg{paramAmount}")
        paramAmount += 1
    str_extern_linker += ", ".join(param_list) + "):\n"
    str_extern_linker += "\treturn extern_" + fn_name + "(" + ", ".join(param_list) + ")\n\n"
    return str_extern_linker



def get_str_return_type(fn) -> str:
    sig = inspect.signature(fn)
    return_type = sig.return_annotation
    return return_type.__name__

def get_str_param_list(fn) -> str:
    sig = inspect.signature(fn)
    params = []
    for name, param in sig.parameters.items():
        param_type = param.annotation.__name__
        if (param_type == "CONTEXT"):
            param_type = "void*"
            continue
        if (param_type == "str"):
            param_type = "std::string"
            params.append(f"{param_type} {name}")
        else:
            params.append(f"{param_type} {name}")
    return ", ".join(params)



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

def to_pascal_case(name: str) -> str:
    return "".join(word.capitalize() for word in name.split("_"))

def make_func_component_str(fn, fn_name):
    sig = inspect.signature(fn)
    str_cpp_extern_func = "extern \"C\" int " + fn_name + "(" + get_str_return_type(fn) + "* ret"
    param_list = get_str_param_list(fn)
    if not param_list:
        str_cpp_extern_func += ");"
    else:
        str_cpp_extern_func += ", " + param_list + ");"
    str_cpp_component = "template <typename CONTEXT>\nstruct Impl" + to_pascal_case(fn_name) + "{\n\t" + get_str_return_type(fn) + " call("
    if not param_list:
        str_cpp_component += ")"
        str_cpp_component += get_str_param_list(fn) + "{\n\t\t" + get_str_return_type(fn) + " result;\n\t\t"
        str_cpp_component += fn_name + "(&result, (CONTEXT*)this);\n\t\t"
        str_cpp_component += "return result;\n\t}\n};"
    else:
        body_arg_list = []
        index = 1
        for name, param in sig.parameters.items():
            body_arg_list.append(f"arg{index}")
            index += 1
        str_cpp_component += get_str_param_list(fn) + "){\n\t\t" + get_str_return_type(fn) + " result;\n\t\t"
        str_cpp_component += fn_name + "(&result, (CONTEXT*)this," + ", ".join(body_arg_list) + ");\n\t\t"
        str_cpp_component += "return result;\n\t}\n};"
    full_cpp_code = str_cpp_extern_func + "\n" + str_cpp_component
    return full_cpp_code

def make_func_trait_str(fn, fn_name):
    sig_= inspect.signature(fn)
    str_cpp_trait = ""
    str_cpp_trait += "struct " + to_pascal_case(fn_name) + "{\n\t"
    str_cpp_trait += "struct CallFn{};\n\t"
    str_cpp_trait += "typedef StaticTable<" + "\n\t\tcontainer::Binding<CallFn, int* (" + get_str_return_type(fn) + "*, "
    param_type_list = get_param_type_list(fn)
    if not param_type_list:
        str_cpp_trait += ")>>\n\t"
    else:
        str_cpp_trait += ", ".join(param_type_list) + ")>>\n\t"
    str_cpp_trait += "STable;\n" + "};"

    return str_cpp_trait

def make_cpp_dict(fn_list):
    cpp_dict = {}
    cpp_dict["RequirementSet"] = {}
    cpp_dict["RequiredTraits"] = []
    for item in fn_list:
        cpp_dict["RequirementSet"][item.__name__] = [make_func_trait_str(item, item.__name__), make_func_component_str(item, item.__name__)]
        cpp_dict["RequiredTraits"].append(to_pascal_case(item.__name__))
    print(cpp_dict)
    return cpp_dict


'''
@numba.njit 
def construct():
    return extern_construct()

@numba.njit
def alloc_bytes(ptr, arg1):
    return extern_Alloc_alloc___alloc_fun(ptr, arg1)

@numba.njit
def free_bytes(ptr, arg1):
    return extern_Alloc_alloc___free_fun(ptr, arg1)

@numba.njit
def trait_a_fn(ptr, arg1, arg2):
    return extern_TraitA_trait_a___a_fn(ptr, arg1, arg2)

@numba.njit
def ret_str(ptr):
    return extern_TraitA_trait_a___a_get_str(ptr)
def trait_b_fn(ptr, arg1):
    return extern_TraitB_trait_b___b_fn(ptr, arg1)

@numba.njit
def destruct(ptr):
    return extern_destruct(ptr)
'''
def make_ffi_wrapper_str(fn):
    str_cpp_ffi = "FFI(" + (to_pascal_case(fn.__name__)) + ")"
    return str_cpp_ffi

def make_module_for_fn_str(fn_name):
    str_fn_module = ""
    str_fn_module += "using " + to_pascal_case(fn_name) + "Module = context::SimpleModule<\n\t"
    str_fn_module += "Meta<Impl" + to_pascal_case(fn_name) + ">,\n\t"
    str_fn_module += "context::RequirementSet<>,\n\t"
    str_fn_module += "context::ImplementationSet<" + to_pascal_case(fn_name) + ", FFIEntry<" + to_pascal_case(fn_name) + ">>\n"
    str_fn_module += ">;\n\n"
    return str_fn_module

'''
using BModule = context::SimpleModule<
    Meta<BImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<TraitB, FFIEntry<TraitB>>>;
'''
def make_logic_h_include():
    str_logic_h_include = ""
    str_logic_h_include += "#include <typeinfo>\n"
    str_logic_h_include += "#include \"../../../include/include.h\"\n\n"
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



def make_fn_struct_in_logic_h():
    str_fn_struct_in_logic_h = ""
    str_fn_struct_in_logic_h += "template <auto... THING>\n"
    str_fn_struct_in_logic_h += "struct Fn\n"
    str_fn_struct_in_logic_h += "{\n"
    str_fn_struct_in_logic_h += "};\n\n"
    str_fn_struct_in_logic_h += "template <typename RES, typename CLASS, typename... ARGS, RES (CLASS::*X)(ARGS...)>\n"
    str_fn_struct_in_logic_h += "struct Fn<X>\n"
    str_fn_struct_in_logic_h += "{\n"
    str_fn_struct_in_logic_h += "\tstatic constexpr auto value = X;\n"
    str_fn_struct_in_logic_h += "\ttypedef RES Result;\n"
    str_fn_struct_in_logic_h += "\ttypedef CLASS Class;\n"
    str_fn_struct_in_logic_h += "\ttypedef container::TypeArray<ARGS...> Args;\n"
    str_fn_struct_in_logic_h += "\ttypedef RES (CLASS::*type)(ARGS...);\n\n"
    str_fn_struct_in_logic_h += "\tstatic auto call(CLASS *self, ARGS... args)\n"
    str_fn_struct_in_logic_h += "\t{\n"
    str_fn_struct_in_logic_h += "\t\treturn (self->*X)(args...);\n"
    str_fn_struct_in_logic_h += "\t}\n"
    str_fn_struct_in_logic_h += "};\n\n"
    str_fn_struct_in_logic_h += "template <typename TYPE>\n"
    str_fn_struct_in_logic_h += "struct IsFn\n"
    str_fn_struct_in_logic_h += "{\n"
    str_fn_struct_in_logic_h += "\tstatic constexpr bool value = false;\n"
    str_fn_struct_in_logic_h += "};\n\n"
    str_fn_struct_in_logic_h += "template <auto ARG>\n"
    str_fn_struct_in_logic_h += "struct IsFn<Fn<ARG>>\n"
    str_fn_struct_in_logic_h += "{\n"
    str_fn_struct_in_logic_h += "\tstatic constexpr bool value = true;\n"
    str_fn_struct_in_logic_h += "};\n\n"
    return str_fn_struct_in_logic_h

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

'''
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
'''

def make_logic_h(fn_list):
    include_str = make_logic_h_include()
    fn_struct_str = make_fn_struct_in_logic_h()
    ffi_trait_str = make_ffi_trait_struct_in_logic_h()
    static_table_str = make_static_table_struct_in_logic_h()
    pure_fn_eq_str = make_pure_fn_eq_struct_in_logic_h()

    cpp_dict = make_cpp_dict(fn_list)

    with open ("logic.h", "w") as f:
        f.write(include_str)
        f.write("\n")
        f.write(fn_struct_str)
        f.write("\n")
        f.write(ffi_trait_str)
        f.write("\n")   
        f.write(static_table_str)
        f.write("\n")   
        for name, code_body in cpp_dict["RequirementSet"].items():
            f.write(code_body[0])
            f.write("\n")
        f.write("\n")
        for name, code_body in cpp_dict["RequirementSet"].items():
            f.write(code_body[1])
            f.write("\n")
            f.write(make_module_for_fn_str(name))
        f.write("\n")
        f.write(pure_fn_eq_str)

def make_main_cpp(fn_list):
    str_main_cpp = ""
    str_main_cpp += "#include <iostream>\n"
    str_main_cpp += "#include <fstream>\n"
    str_main_cpp += "#include <unistd.h>\n"
    str_main_cpp += "#include \"logic.h\"\n\n"


    str_main_cpp += "using RootModule = context::ModuleBundle<"
    fn_name_list = []
    for fn_name in fn_list:
        fn_name_list.append(to_pascal_case(fn_name.__name__))
    
    str_main_cpp += ", ".join(fn_name_list) + ">;\n\n"

    str_main_cpp += "template <typename CTX>\n"
    str_main_cpp += "void run()\n{\n" 
    str_main_cpp += "\tif constexpr (CTX::Info::SATISFIED){\n\t"
    str_main_cpp += "\tCTX ctx{};\n\t"
    str_main_cpp += "\tas<FFIGen>(ctx).genffi(\"logic.h\");\n\t"
    str_main_cpp += "}\n"
    str_main_cpp += "}\n\n"
    str_main_cpp += "int main()\n{\n"
    str_main_cpp += "\ttypedef typename context::CreateContextType<\n\t\tRootModule,\n\t\tcontainer::TypeSet<"
    str_main_cpp += ", ".join(fn_name_list)
    str_main_cpp += ", FFIEntry<" + ">, FFIEntry<".join(fn_name_list) + ">, FFIGen"
    str_main_cpp += ">,\n\t\tMeta<context::EagerSolve>>::type Ctx;\n\n"
    str_main_cpp += "\trun<Ctx>();\n"
    str_main_cpp += "\treturn 0;\n"
    str_main_cpp += "}\n"

    with open ("main.cpp", "w") as f:
        f.write(str_main_cpp)
    
make_logic_h([add_one, add_it])




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