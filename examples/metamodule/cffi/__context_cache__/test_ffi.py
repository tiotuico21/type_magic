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

extern__TYPEMAGICNAddOne6CallFnE = numba.types.ExternalFunction(
	"_TYPEMAGICNAddOne6CallFnE",
	numba.core.typing.signature(
		numba.types.float32, 
		numba.types.voidptr, numba.types.float32, numba.types.boolean, numba.types.float32
	)
)

extern__TYPEMAGICNAddIt6CallFnE = numba.types.ExternalFunction(
	"_TYPEMAGICNAddIt6CallFnE",
	numba.core.typing.signature(
		numba.types.int32, 
		numba.types.voidptr
	)
)

extern__TYPEMAGICNSubArgs6CallFnE = numba.types.ExternalFunction(
	"_TYPEMAGICNSubArgs6CallFnE",
	numba.core.typing.signature(
		numba.types.float32, 
		numba.types.voidptr, numba.types.float32, numba.types.boolean, numba.types.float32
	)
)

extern__TYPEMAGICNIsTrue6CallFnE = numba.types.ExternalFunction(
	"_TYPEMAGICNIsTrue6CallFnE",
	numba.core.typing.signature(
		numba.types.boolean, 
		numba.types.voidptr, numba.types.boolean
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
	return extern__TYPEMAGICNAddOne6CallFnE(ptr, arg1, arg2, arg3)

@numba.njit(cache=False)
def add_it(ptr):
	return extern__TYPEMAGICNAddIt6CallFnE(ptr)

@numba.njit(cache=False)
def sub_args(ptr, arg1, arg2, arg3):
	return extern__TYPEMAGICNSubArgs6CallFnE(ptr, arg1, arg2, arg3)

@numba.njit(cache=False)
def is_true(ptr, arg1):
	return extern__TYPEMAGICNIsTrue6CallFnE(ptr, arg1)

@numba.njit(cache=False)
def print_int(ptr, arg1):
	return extern__TYPEMAGICN5PrintIiE7PrintFnE(ptr, arg1)

