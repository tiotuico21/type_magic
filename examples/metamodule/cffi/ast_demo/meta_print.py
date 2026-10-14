
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


class MyMetaPrint(object):
    def __init__(self, kind):
        self.kind = kind

    def __repr__(self):
        return f"MyMetaPrint({self.kind})"


class MyMetaPrintType(types.Type):
    def __init__(self, kind):
        self.kind = kind
        super(MyMetaPrintType, self).__init__(
            name=f"MyMetaPrintType({kind})"
        )
_meta_print_set = {}


@staticmethod
def _meta_print_type(kind):
    if not kind in _meta_print_set:
        _meta_print_set[kind] = MyMetaPrintType(kind)

    return _meta_print_set[kind]


record_type = nb.from_dtype(
    np.dtype([
        ('first_arg', np.float64),
        ('second_arg', np.int64)
    ])
)

_meta_print_ext_map = {
    _meta_print_type(nb.types.int32): nb.types.ExternalFunction(
        "_TYPEMAGICNPrintIiE7PrintFnE",
        nb.core.typing.signature(
            nb.types.void,
            nb.types.voidptr, nb.types.int32
        )
    ),
    _meta_print_type(nb.types.float32): nb.types.ExternalFunction(
        "_TYPEMAGICNPrintIfE7PrintFnE",
        nb.core.typing.signature(
            nb.types.void,
            nb.types.voidptr, nb.types.float32
        )
    ),
}



@overload_method(MyMetaPrintType, '__call__')
def call_overload_ffi(self, ctx, val):
    extern_fn = _meta_print_ext_map[self]

    def impl(self, ctx, val):
        return extern_fn(ctx, val)

    return impl


@lower_builtin(MyMetaPrintType, MyMetaPrintType, types.VarArg(types.Any))
def method_impl(context, builder, sig, args):

    typing_context = context.typing_context
    overload = call_overload_ffi

    fnty = typing_context.resolve_value_type(overload)

    sig = fnty.get_call_type(
        typing_context,
        sig.args,
        {}
    )

    sig = sig.replace(
        pysig=nb.core.utils.pysignature(overload)
    )

    call = context.get_function(fnty, sig)

    context.add_linking_libs(
        getattr(call, 'libs', ())
    )

    return call(builder, args)


@typeof_impl.register(MyMetaPrint)
def typeof_index(val, c):
    return _meta_print_type(val.kind)


@type_callable(MyMetaPrint)
def type__meta_print(context):

    valid_type_set = set([
        nb.types.Integer,
        nb.types.Float,
        nb.types.Boolean,
        nb.types.functions.NumberClass,
        nb.types.int64,
        nb.types.int32,
        nb.types.float64,
        nb.types.float32,
        record_type,
        nb.types.void
    ])


    def typer(kind):

        if isinstance(kind, nb.types.TypeRef):
            kind = kind.instance_type

        if isinstance(kind, nb.types.NumberClass):
            kind = kind.instance_type

        if isinstance(kind, nb.types.Record):
            print("THIS IS A RECORD")

        if kind in valid_type_set:
            print("________________________")
            print("ENTERING VALID TYPE BRANCH")
            print("************vali type")
            print("")

            print("88888888888888")
            print(kind)
            print("88888888888888")

            print(_meta_print_type(kind))

            print("________________________")
            print("")

            return _meta_print_type(kind)

        else:
            raise NumbaTypeError(
                f"Type {kind} not in type set"
            )

    return typer


@register_model(MyMetaPrintType)
class MyMetaPrintModel(models.StructModel):

    def __init__(self, dmm, fe_type):
        members = []

        models.StructModel.__init__(
            self,
            dmm,
            fe_type,
            members
        )


make_attribute_wrapper(
    MyMetaPrintType,
    'kind',
    'kind'
)


@lower_builtin(MyMetaPrint, nb.types.Any)
def impl__meta_print(context, builder, sig, args):

    typ = sig.return_type

    _meta_print = cgutils.create_struct_proxy(typ)(
        context,
        builder
    )

    return _meta_print._getvalue()


@unbox(MyMetaPrintType)
def unbox__meta_print(typ, obj, c):

    is_error_ptr = cgutils.alloca_once_value(
        c.builder,
        cgutils.false_bit
    )

    _meta_print = cgutils.create_struct_proxy(typ)(
        c.context,
        c.builder
    )

    return NativeValue(
        _meta_print._getvalue(),
        is_error=c.builder.load(is_error_ptr)
    )


@box(MyMetaPrintType)
def box__meta_print(typ, val, c):

    print(f"type from boxing: {typ}")

    ret_ptr = cgutils.alloca_once(
        c.builder,
        c.pyapi.pyobj
    )

    fail_obj = c.pyapi.get_null_object()

    with ExitStack() as stack:

        class_obj = c.pyapi.unserialize(
            c.pyapi.serialize_object(MyMetaPrint)
        )

        with cgutils.early_exit_if_null(
            c.builder,
            stack,
            class_obj
        ):
            c.builder.store(
                fail_obj,
                ret_ptr
            )

        kind_obj = c.pyapi.unserialize(
            c.pyapi.serialize_object(typ.kind)
        )

        res = c.pyapi.call_function_objargs(
            class_obj,
            (kind_obj,)
        )

        c.pyapi.decref(kind_obj)
        c.pyapi.decref(class_obj)

        c.builder.store(
            res,
            ret_ptr
        )

    return c.builder.load(ret_ptr)


@nb.njit
def makeInstance(x):
    retInstance = MyMetaPrint(x)
    return retInstance


@nb.njit
def test__meta_print(__meta_print_instance, ctx, x):
    __meta_print_instance(ctx, x)

