import re
import sys
import inspect
import subprocess
import numba as nb

from llvmlite import binding

binding.load_library_permanently("./my_dynamic_library.so")


extern_construct = nb.types.ExternalFunction(
	"construct",
	nb.core.typing.signature(nb.types.voidptr)
)

@nb.njit(cache=False)
def construct():
	return extern_construct()

import meta_print
import meta_log
extern__TYPEMAGICNAddOne6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNAddOne6CallFnE",
	nb.core.typing.signature(
		nb.types.float32, 
		nb.types.voidptr, nb.types.float32, nb.types.boolean, nb.types.float32
	)
)

@nb.njit(cache=False)
def add_one(ptr, arg1, arg2, arg3):
	return extern__TYPEMAGICNAddOne6CallFnE(ptr, arg1, arg2, arg3)

extern__TYPEMAGICNAddIt6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNAddIt6CallFnE",
	nb.core.typing.signature(
		nb.types.int32, 
		nb.types.voidptr
	)
)

@nb.njit(cache=False)
def add_it(ptr):
	return extern__TYPEMAGICNAddIt6CallFnE(ptr)

extern__TYPEMAGICNSubArgs6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNSubArgs6CallFnE",
	nb.core.typing.signature(
		nb.types.float32, 
		nb.types.voidptr, nb.types.float32, nb.types.boolean, nb.types.float32
	)
)

@nb.njit(cache=False)
def sub_args(ptr, arg1, arg2, arg3):
	return extern__TYPEMAGICNSubArgs6CallFnE(ptr, arg1, arg2, arg3)

extern__TYPEMAGICNIsTrue6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNIsTrue6CallFnE",
	nb.core.typing.signature(
		nb.types.boolean, 
		nb.types.voidptr, nb.types.boolean
	)
)

@nb.njit(cache=False)
def is_true(ptr, arg1):
	return extern__TYPEMAGICNIsTrue6CallFnE(ptr, arg1)

extern__TYPEMAGICNEven6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNEven6CallFnE",
	nb.core.typing.signature(
		nb.types.int32, 
		nb.types.voidptr, nb.types.int32
	)
)

@nb.njit(cache=False)
def even(ptr, arg1):
	return extern__TYPEMAGICNEven6CallFnE(ptr, arg1)

extern__TYPEMAGICNOdd6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNOdd6CallFnE",
	nb.core.typing.signature(
		nb.types.int32, 
		nb.types.voidptr, nb.types.int32
	)
)

@nb.njit(cache=False)
def odd(ptr, arg1):
	return extern__TYPEMAGICNOdd6CallFnE(ptr, arg1)

extern__TYPEMAGICN2StoppingTime6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICN2StoppingTime6CallFnE",
	nb.core.typing.signature(
		nb.types.int32, 
		nb.types.voidptr, nb.types.int32
	)
)

@nb.njit(cache=False)
def stopping_time(ptr, arg1):
	return extern__TYPEMAGICN2StoppingTime6CallFnE(ptr, arg1)

extern__TYPEMAGICNSubOne6CallFnE = nb.types.ExternalFunction(
	"_TYPEMAGICNSubOne6CallFnE",
	nb.core.typing.signature(
		nb.types.float32, 
		nb.types.voidptr, nb.types.float32
	)
)

@nb.njit(cache=False)
def sub_one(ptr, arg1):
	return extern__TYPEMAGICNSubOne6CallFnE(ptr, arg1)

extern_destruct = nb.types.ExternalFunction(
	"destructor",
	nb.core.typing.signature(
		nb.types.voidptr, nb.types.voidptr
	)
)

@nb.njit(cache=False)
def destruct(ptr):
	return extern_destruct(ptr)

