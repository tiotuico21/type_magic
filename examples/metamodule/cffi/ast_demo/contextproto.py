import plug_in
import bindings
import numba


class CONTEXT:
    pass

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





