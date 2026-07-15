import re
import sys
import numba
import inspect
import subprocess

from llvmlite import binding

binding.load_library_permanently("../my_dynamic_library.so")

extern_construct = numba.types.ExternalFunction(
    "construct",
    numba.core.typing.signature(
        numba.types.voidptr
    )
)
extern_allocate_bytes = numba.types.ExternalFunction(
    "_TYPEMAGICN5Alloc8AllocFunE",
    numba.core.typing.signature(
        numba.types.voidptr,
        numba.types.voidptr,
        numba.types.long_ #int64??????
    )
)

extern_free_bytes = numba.types.ExternalFunction(
    "_TYPEMAGICN5Alloc7FreeFunE",
    numba.core.typing.signature(
        numba.types.void,
        numba.types.voidptr,
        numba.types.voidptr
    )
)

extern_trait_a_fn = numba.types.ExternalFunction(
    "_TYPEMAGICN6TraitA3AFnE",
    numba.core.typing.signature(
        numba.types.void,
        numba.types.voidptr,
        numba.types.int32,
        numba.types.bool #boolean?????
    )
)

extern_ret_str = numba.types.ExternalFunction(
    "_TYPEMAGICN6TraitA7AGetStrE",
    numba.core.typing.signature(
        numba.types.string,
        numba.types.voidptr
    )
)

extern_trait_b_fn = numba.types.ExternalFunction(
    "_TYPEMAGICN6TraitB3BFnE",
    numba.core.typing.signature(
        numba.types.float32,
        numba.types.voidptr,
        numba.types.double
    )
)

extern_destruct = numba.types.ExternalFunction(
    "destructor",
    numba.core.typing.signature(
        numba.types.void,
        numba.types.voidptr
    )
)

@numba.njit 
def construct():
    return extern_construct();

@numba.njit
def alloc_bytes(ptr, arg1):
    return extern_allocate_bytes(ptr, arg1)

@numba.njit
def free_bytes(ptr, arg1):
    return extern_free_bytes(ptr, arg1)

@numba.njit
def trait_a_fn(ptr, arg1, arg2):
    return extern_trait_a_fn(ptr, arg1, arg2)

@numba.njit
def ret_str(ptr):
    return extern_ret_str(ptr)

@numba.njit
def trait_b_fn(ptr, arg1):
    return extern_trait_b_fn(ptr, arg1)

@numba.njit
def destruct(ptr):
    return extern_destruct(ptr)



ctx = construct();


#ptr = alloc_bytes(ctx, 5)

#free_bytes(ctx, ptr)

trait_a_fn(ctx, 5, True)

print(trait_b_fn(ctx, 4))

#print(ret_str(ctx))

destruct(ctx)