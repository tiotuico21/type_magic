#include <iostream>

static std::string meta_code = R"PY(
from numba import types
from numba.extending import typeof_impl
from numba.extending import as_numba_type
from numba.extending import type_callable
from numba.extending import models, register_model
from numba.extending import make_attribute_wrapper
from numba.extending import overload_attribute
from numba.extending import lower_builtin
from numba.core import cgutils
from numba.extending import unbox, NativeValue
from contextlib import ExitStack
from numba.extending import box
from numba import njit
from numba.core.errors import NumbaTypeError
from numba.core.extending import overload_method
import numba as nb
import numpy as np
from numpy import int64
import re
import sys
import inspect
import subprocess

from llvmlite import binding

binding.load_library_permanently("./my_dynamic_library.so")


class My__TYPE__(object):
    def __init__(self, kind):
        self.kind = kind

    def __repr__(self):
        return f"My__TYPE__({self.kind})"


class My__TYPE__Type(types.Type):
    def __init__(self, kind):
        self.kind = kind
        super(My__TYPE__Type, self).__init__(
            name=f"My__TYPE__Type({kind})"
        )
__NAME___set = {}


@staticmethod
def __NAME___type(kind):
    if not kind in __NAME___set:
        __NAME___set[kind] = My__TYPE__Type(kind)

    return __NAME___set[kind]


record_type = nb.from_dtype(
    np.dtype([
        ('first_arg', np.float64),
        ('second_arg', np.int64)
    ])
)
)PY";