import plug_in
from harmonize import bindings
import numba
import sys

# print("\n========== TEST 2 ==========")

# print("bindings:", bindings)
# print("bindings id:", id(bindings))
# print("bindings spec:", bindings.__spec__)

# print("\nLoading python_bindings...")

# pb = bindings.python_bindings

# print("python_bindings:", pb)
# print("python_bindings name:", pb.__name__)
# print("python_bindings file:", pb.__file__)
# print("python_bindings id:", id(pb))

# print("\nChecking sys.modules:")

# print(
#     "Registered:",
#     sys.modules.get("bindings.python_bindings")
# )

# print(
#     "Same object:",
#     sys.modules.get("bindings.python_bindings") is pb
# )

# print("\nChecking add_it:")

# print("add_it:", pb.add_it)
# print("type:", type(pb.add_it))
# print("signatures:", pb.add_it.signatures)

# # print("\n========== TEST 3 ==========")

# # @numba.njit(cache=False)
# # def test_direct(ptr):
# #     return bindings.python_bindings.add_it(ptr)

# # print("Created test_direct")
# # test_direct.compile((numba.types.voidptr,))

# # print("DIRECT COMPILATION SUCCESSFUL")
# # print("Signatures:", test_direct.signatures)

# print("\n========== TEST 4A ==========")

# @numba.njit(cache=False)
# def test_module_probe(x):
#     return bindings.python_bindings.module_probe(x)

# print("Created test_module_probe")

# result = test_module_probe(10)

# print("Result:", result)
# print("Signatures:", test_module_probe.signatures)

# print("\n========== TEST 5 ==========")

# from numba.core import types

# print("External function type:")
# print(pb.extern__TYPEMAGICNAddIt6CallFnE)

# print("\nWrapper dispatcher:")
# print(pb.add_it)

# print("\nCompiling add_it...")

# try:
#     pb.add_it.compile((types.voidptr,))
#     print("Compilation succeeded")
#     print(pb.add_it.inspect_llvm((types.voidptr,)))
# except Exception as e:
#     print("Python exception:", repr(e))

# print("\n========== TEST 6 ==========")

# @numba.njit(cache=False)
# def test_direct(ptr):
#     return pb.add_it(ptr)

# print("Compiling outer function...")

# try:
#     test_direct.compile((numba.types.voidptr,))
#     print("Outer compilation succeeded")

#     print(test_direct.inspect_llvm(
#         (numba.types.voidptr,)
#     ))

# except Exception as e:
#     print("Python exception:", repr(e))

# print("\n========== TEST 7 ==========")

# @numba.njit(cache=False)
# def test_direct_external(ptr):
#     return pb.extern__TYPEMAGICNAddIt6CallFnE(ptr)

# print("Compiling direct external call...")

# test_direct_external.compile((numba.types.voidptr,))

# print("Compilation succeeded")

# print(test_direct_external.inspect_llvm(
#     (numba.types.voidptr,)
# ))

class CONTEXT:
    pass

@numba.njit()
def my_print(value: int):
    pass

def add_one(ctx: CONTEXT, arg1: float, arg2: bool, arg3: float) -> float:
    # #imported_ctx = plug_in.test_ffi.construct()
    # val = bindings.python_bindings.add_it(ctx)
    if (arg2):
        return arg1 
    # return val + arg3
    return arg1 + arg3


def add_it(ctx: CONTEXT) -> int:
    return 8

def even(ctx: CONTEXT, arg1: int) -> int:
    # arg1 = arg1 // 2
    # printInstance = bindings.python_bindings.meta_print.makeInstance(numba.types.int32)
    # bindings.python_bindings.meta_print.test__meta_print(printInstance, ctx, arg1)
    # if (arg1 % 2 == 0):
    #     iter = bindings.python_bindings.even(ctx, arg1) 
    # elif arg1 != 1:
    #     iter = bindings.python_bindings.odd(ctx, arg1) 
    # return iter + 1
    return arg1 % 2 == 0
    
def odd(ctx: CONTEXT, arg1: int) -> int:
    #imported_ctx = magic.test_ffi.construct()
    #arg1  = arg1 * 3 + 1
    #iter = bindings.python_bindings.even(ctx, arg1)
    #return iter + 1
    arg1 = arg1 * 3 + 1
    return arg1 + 1

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





