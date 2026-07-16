import numba
from numba import types
from numba import cfunc
from llvmlite import binding
import inspect 
#from type_magic import CONTEXT

def add_one(arg1:int, arg2: str, arg3: bool) -> int:
    return x + 1

def add_it() -> int:
    return 1

def get_str_return_type(fn) -> str:
    sig = inspect.signature(fn)
    return_type = sig.return_annotation
    return return_type.__name__

def get_str_param_list(fn) -> str:
    sig = inspect.signature(fn)
    params = []
    for name, param in sig.parameters.items():
        param_type = param.annotation.__name__
        params.append(f"{param_type} {name}")
    return ", ".join(params)

def get_param_type_list(fn):
    sig = inspect.signature(fn)
    param_types = []
    for name, param in sig.parameters.items():
        param_types.append(f"{param.annotation.__name__}")
    return param_types

def to_pascal_case(name: str) -> str:
    return "".join(word.capitalize() for word in name.split("_"))

def make_func_component_str(fn, fn_name):
    sig = inspect.signature(fn)
    str_cpp_extern_func = "extern \"C\" int " + fn_name + "(" + get_str_return_type(fn) + "* ret, void* ctx"
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

    print(full_cpp_code)
def make_func_trait_str(fn, fn_name):
    sig_= inspect.signature(fn)
    str_cpp_trait = ""
    str_cpp_trait += "struct " + to_pascal_case(fn_name) + "{\n\t"
    str_cpp_trait += "struct CallFn{};\n\t"
    str_cpp_trait += "typedef StaticTable<" + "\n\t\tcontainer::Binding<CallFn, int* (" + get_str_return_type(fn) + "*, void*"
    param_type_list = get_param_type_list(fn)
    if not param_type_list:
        str_cpp_trait += ")>>\n\t"
    else:
        str_cpp_trait += ", " + ", ".join(param_type_list) + ")>>\n\t"
    str_cpp_trait += "STable;\n" + "}"

    print(str_cpp_trait)

def make_cpp_dict(fn_list):
    cpp_dict = {}
    cpp_dict["RequirementSet"] = {}
    cpp_dict["RequiredTraits"] = []
    for item in fn_list:
        dict_entry = {item: [make_func_trait_str(item, item.annotations.__name__), make_func_component_str(item, item.annotations.__name__)]}
        cpp_dict["RequirementSet"][item.annotations.__name__] = dict_entry
        cpp_dict["RequiredTraits"].append(to_pascal_case(item.annotations.__name__))

def make_cpp(fn_list):
    cpp_dict = make_cpp_dict(fn_list)

    with open ("logic.cpp", "w") as f:
        for name, code_body in cpp_dict.items():
            f.write(code_body[0])
            f.write("\n")
        for name, code_body in cpp_dict.items():
            f.write(code_body[1])
            f.write("\n")
make_func_component_str(add_one, "add_one")

