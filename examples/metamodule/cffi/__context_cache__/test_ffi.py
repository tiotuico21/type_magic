import re
import sys
import numba
import inspect
import subprocess

from llvmlite import binding

binding.load_library_permanently("./my_dynamic_library.so")
extern_construct = numba.types.ExternalFunction(
	"construct",
	numba.core.typing.signature(numba.types.voidptr)
)

extern_destruct = numba.types.ExternalFunction(
	"destructor",
	numba.core.typing.signature(
		numba.types.voidptr, numba.types.voidptr
	)
)

extern_add_one___call_fn = numba.types.ExternalFunction(
	"_TYPEMAGICN6AddOne6CallFnE",
	numba.core.typing.signature(
		numba.types.float32, 
		numba.types.voidptr, numba.types.float32, numba.types.boolean, numba.types.float32
	)
)

extern_add_it___call_fn = numba.types.ExternalFunction(
	"_TYPEMAGICN5AddIt6CallFnE",
	numba.core.typing.signature(
		numba.types.int32, 
		numba.types.voidptr
	)
)

@numba.njit(cache=False)
def construct():
	return extern_construct()

@numba.njit(cache=False)
def destruct(ptr):
	return extern_destruct(ptr)

@numba.njit(cache=False)
def add_one(ptr, arg1, arg2, arg3):
	return extern_add_one___call_fn(ptr, arg1, arg2, arg3)

@numba.njit(cache=False)
def add_it(ptr):
	return extern_add_it___call_fn(ptr)

