import re
import sys
import numba
import inspect
import subprocess

from llvmlite import binding

binding.load_library_permanently("./my_dynamic_library.so")
extern_contruct = numba.types.ExternalFunction(
	"construct",
	numba.core.typing.signature(
		numba.types.voidptr
	)
)

extern_destruct = numba.types.ExternalFunction(
	"destruct",
	numba.core.typing.signature(
		numba.types.void,
		numba.types.voidptr
	)
)

extern_alloc___alloc_fun = numba.types.ExternalFunction(
	"_TYPEMAGICN5Alloc8AllocFunE",
	numba.core.typing.signature(
		numba.types.voidptr,
		numba.types.voidptr, numba.types.uint64
	)
)

extern_alloc___free_fun = numba.types.ExternalFunction(
	"_TYPEMAGICN5Alloc7FreeFunE",
	numba.core.typing.signature(
		numba.types.void,
		numba.types.voidptr, numba.types.voidptr
	)
)

extern_trait_a___a_fn = numba.types.ExternalFunction(
	"_TYPEMAGICN6TraitA3AFnE",
	numba.core.typing.signature(
		numba.types.void,
		numba.types.voidptr, numba.types.int32, numba.types.boolean
	)
)

extern_trait_a___a_get_str = numba.types.ExternalFunction(
	"_TYPEMAGICN6TraitA7AGetStrE",
	numba.core.typing.signature(
		numba.CPointer(types.int8),
		numba.types.voidptr
	)
)

extern_trait_b___b_fn = numba.types.ExternalFunction(
	"_TYPEMAGICN6TraitB3BFnE",
	numba.core.typing.signature(
		numba.types.float32,
		numba.types.voidptr, numba.types.float64
	)
)

@numba.njit
def construct():
	return extern_construct()

@numba.njit
def destruct(ptr):
	return extern_destruct(ptr)

@numba.njit
def alloc_bytes(ptr, arg1):
	return extern_alloc___alloc_fun(ptr, arg1)

@numba.njit
def free_bytes(ptr, arg1):
	return extern_alloc___free_fun(ptr, arg1)

@numba.njit
def trait_a_fn(ptr, arg1, arg2):
	return extern_trait_a___a_fn(ptr, arg1, arg2)

@numba.njit
def ret_str(ptr):
	return extern_trait_a___a_get_str(ptr)

@numba.njit
def trait_b_fn(ptr, arg1):
	return extern_trait_b___b_fn(ptr, arg1)

