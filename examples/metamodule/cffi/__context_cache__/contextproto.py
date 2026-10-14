import plug_in

import numba
import sys
import pathlib
import importlib.abc
import importlib.util
from importlib.machinery import ModuleSpec



class CONTEXT:
    pass


class MagicLoader():
    counter = 0

    def create_module(self, spec):
        return None

    def exec_module(self, module):
        def __getattr__(name):
            if not pathlib.Path(f"./{name}.py").is_file():
                print("No module")
                print(f"Name failed to find: {name}\n\n")
                setattr(module, name, None)
                return None
            print("\n\nFOUND MODULE\n\n")
            spec = importlib.util.spec_from_file_location(name, f"./{name}.py")
            mod = importlib.util.module_from_spec(spec)
            sys.modules[f"bindings.{name}"] = mod
            spec.loader.exec_module(mod)

            #sats it returns none
            setattr(module, name, mod)
            return mod
        module.__getattr__ = __getattr__

class MagicFinder():
    @classmethod
    def find_spec(cls, fullname, path, target=None):
        print(f"\n\nENTERED FINDER - FULL NAME: {fullname} Path: {path} \n\n")
        if fullname == "bindings":
            #    magic_name = f"magic.{fullname}"
            return ModuleSpec(fullname, MagicLoader())
        return None


sys.meta_path.insert(0, MagicFinder())

import bindings

@numba.njit()
def my_print(value: int):
    pass

def add_one(ctx: CONTEXT, arg1: float, arg2: bool, arg3: float) -> float:
    #imported_ctx = plug_in.test_ffi.construct()
    val = bindings.python_bindings.add_it(ctx)
    if (arg2):
        return arg1 
    return val + arg3


def add_it(ctx: CONTEXT) -> int:
    return 8

def even(ctx: CONTEXT, arg1: int) -> int:
    arg1 = arg1 // 2
    printInstance = bindings.python_bindings.meta_print.makeInstance(numba.types.int32)
    bindings.python_bindings.meta_print.test__meta_print(printInstance, ctx, arg1)
    if (arg1 % 2 == 0):
        iter = bindings.python_bindings.even(ctx, arg1) 
    elif arg1 != 1:
        iter = bindings.python_bindings.odd(ctx, arg1) 
    return iter + 1
    
def odd(ctx: CONTEXT, arg1: int) -> int:
    #imported_ctx = magic.test_ffi.construct()
    arg1  = arg1 * 3 + 1
    iter = bindings.python_bindings.even(ctx, arg1)
    return iter + 1

def stopping_time(ctx: CONTEXT, arg1: int) -> int:
    imported_ctx = bindings.python_bindings.construct()
    iter = 0
    if arg1 % 2 == 0:
        iter = bindings.python_bindings.even(imported_ctx, arg1) 
    else:
        iter = bindings.python_bindings.odd(imported_ctx, arg1) 
    bindings.python_bindings.destruct(imported_ctx)
    return iter


required_inherited_trait_dict = {"add_one": [plug_in.trait_dict.SubOne["cpp_name"]]}
plug_in.compile_and_run([add_one, add_it, even, odd, stopping_time], [plug_in.trait_dict.SubOne["cpp_name"], plug_in.trait_dict.FFIEntry_SubOne["cpp_name"], plug_in.trait_dict.Print("int"), plug_in.trait_dict.Print("float"), plug_in.trait_dict.FFIEntry(plug_in.trait_dict.Print("int")["cpp_name"]), plug_in.trait_dict.FFIEntry(plug_in.trait_dict.Print("float")["cpp_name"]), plug_in.trait_dict.Log("int"), plug_in.trait_dict.Log("float"), plug_in.trait_dict.FFIEntry(plug_in.trait_dict.Log("int")["cpp_name"]), plug_in.trait_dict.FFIEntry(plug_in.trait_dict.Log("float")["cpp_name"])], "main.cpp", True, required_inherited_trait_dict)





