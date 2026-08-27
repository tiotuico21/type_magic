
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



class MyPrint(object):
    def __init__(self, kind):
        self.kind = kind

    def __repr__(self):
        return f"MyPrint({self.kind})"

class MyPrintType(types.Type):
    def __init__(self,kind):
        self.kind = kind
        #do we need to add a kind
        super(MyPrintType, self).__init__(name=f"MyPrintType({kind})")

my_print_set = {}
@staticmethod
def my_print_type(kind):
    if not kind in my_print_set:
        my_print_set[kind] = MyPrintType(kind)
        
    return my_print_set[kind]        
        
extern_construct = nb.types.ExternalFunction(
	"construct",
	nb.core.typing.signature(nb.types.voidptr)
)

@nb.njit(cache=False)
def construct():
	return extern_construct()

meta_print_ext_map = {
    my_print_type(nb.types.int32): nb.types.ExternalFunction(
        "_TYPEMAGICNPrintIiE7PrintFnE",
        nb.core.typing.signature(
            nb.types.void,
            nb.types.voidptr, nb.types.int32
        )
    ),
    my_print_type(nb.types.float32): nb.types.ExternalFunction(
        "_TYPEMAGICNPrintIfE7PrintFnE",
        nb.core.typing.signature(
            nb.types.void,
            nb.types.voidptr, nb.types.float32
        )
    ),
}
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


record_type = nb.from_dtype(np.dtype([('first_arg', np.float64), ('second_arg', np.int64)]))
#@overload_method(MyPrintType, '__call__')
#def call_overload_2_arg(self, val):
#    extern_fn = my_print_ext_map[self]

@overload_method(MyPrintType, '__call__')
def call_overload_ffi(self, ctx, val):

    extern_fn = my_print_ext_map[self]

    def impl(self, ctx, val):
        return extern_fn(ctx, val)

    return impl
        

@lower_builtin(MyPrintType, MyPrintType, types.VarArg(types.Any))
def method_impl(context, builder, sig, args):
    print("METHOD IMPLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL")
    typing_context = context.typing_context
    overload = call_overload_ffi
    fnty = typing_context.resolve_value_type(overload)
    sig = fnty.get_call_type(typing_context, sig.args, {})
    sig = sig.replace(pysig=nb.core.utils.pysignature(overload))

    call = context.get_function(fnty, sig)

    context.add_linking_libs(getattr(call, 'libs', ()))
    return call(builder, args)



@typeof_impl.register(MyPrint)
def typeof_index(val, c):
    return my_print_type(val.kind)

#as_numba_type.register(MyPrint, my_print_set)




@type_callable(MyPrint)
def type_my_print(context):
    valid_type_set = set([nb.types.Integer,nb.types.Float, nb.types.Boolean,nb.types.functions.NumberClass, nb.types.int64, nb.types.int32, nb.types.float64, nb.types.float32, record_type, nb.types.void])
    print(f"Valid type set is : {valid_type_set}")
    def typer(kind):
        print("")
        print("")
        print("___________________")
        print("NON INSTANCE TYPE")
        print(f"FOUND KIND: {kind} with type {type(kind)}")
        if isinstance(kind,nb.types.TypeRef):
            print("THIS IS A TYPEREF")
            kind = kind.instance_type
        if (isinstance(kind, nb.types.NumberClass)):
            kind = kind.instance_type
            print("______________________")
            print("INSTANCE TYPE")
            print(kind)
            print("______________________")
            print("")
        if (isinstance(kind, nb.types.Record)):
            print("THIS IS A RECORD")
            
        if kind in valid_type_set:
                    print("_____________________________________________________")
                    print("ENTERING VALID TYPE BRANCH")
                    print("*************vali type")
                    print("")
                    print("88888888888888")
                    print(kind)
                    print("88888888888888")
                    print(my_print_type(kind))
                    print("_____________________________________________________")
                    print("")
                    return my_print_type(kind)
                    
        else:
            raise NumbaTypeError(f"Type {kind} not in type set")
    return typer 

@register_model(MyPrintType)
class MyPrintModel(models.StructModel):
    #idt kind is part of self yet
    def __init__(self, dmm, fe_type):
        # we were trying to read directly in members for kind
        members = []
        models.StructModel.__init__(self, dmm, fe_type, members)

#we need this right
make_attribute_wrapper(MyPrintType, 'kind', 'kind')


@lower_builtin(MyPrint, nb.types.Any)
def impl_myprint(context, builder, sig, args):
    typ = sig.return_type
    #kind = args[0]
    myprint = cgutils.create_struct_proxy(typ)(context, builder)
    #  myprint.kind = kind
    return myprint._getvalue()


#do i have to specify multipke typles when assigning myprint
@unbox(MyPrintType)
def unbox_interval(typ, obj, c):
    """
    Convert a Interval object to a native interval structure.
    """
    is_error_ptr = cgutils.alloca_once_value(c.builder, cgutils.false_bit)
    myprint = cgutils.create_struct_proxy(typ)(c.context, c.builder)

    #with ExitStack() as stack:
    #    kind_obj = c.pyapi.object_getattr_string(obj, "kind")
    #    with cgutils.early_exit_if_null(c.builder, stack, kind_obj):
    #       c.builder.store(cgutils.true_bit, is_error_ptr)
    #    kind_native = c.unbox(nb.types.Any, kind_obj)
    #    c.pyapi.decref(kind_obj)
    #    with cgutils.early_exit_if(c.builder, stack, kind_native.is_error):
    #       c.builder.store(cgutils.true_bit, is_error_ptr)

     
    #myprint.kind = kind_native.value
       

    return NativeValue(myprint._getvalue(), is_error=c.builder.load(is_error_ptr))
        
@box(MyPrintType)
def box_interval(typ, val, c):
    print(f"type from boxing: {typ}")
  
    ret_ptr = cgutils.alloca_once(c.builder, c.pyapi.pyobj)
    fail_obj = c.pyapi.get_null_object()

    with ExitStack() as stack:
        #myprint = cgutils.create_struct_proxy(typ)(c.context, c.builder, value=val)
        #kind_obj = c.box(nb.types.Any, myprint.kind)

        #with cgutils.early_exit_if_null(c.builder, stack, kind_obj):
        #    c.builder.store(fail_obj, ret_ptr)

        class_obj = c.pyapi.unserialize(c.pyapi.serialize_object(MyPrint))
        with cgutils.early_exit_if_null(c.builder, stack, class_obj):
            #c.pyapi.decref(kind_obj)
            c.builder.store(fail_obj, ret_ptr)

        kind_obj = c.pyapi.unserialize(
            c.pyapi.serialize_object(typ.kind)
        )

        #res = c.pyapi.call_function_objargs(class_obj, (kind_obj))
        res = c.pyapi.call_function_objargs(class_obj, (kind_obj,))
        c.pyapi.decref(kind_obj)
        c.pyapi.decref(class_obj)
        c.builder.store(res, ret_ptr)

    return c.builder.load(ret_ptr)   
        
        
        
