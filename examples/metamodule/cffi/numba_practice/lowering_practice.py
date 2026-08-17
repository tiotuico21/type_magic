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

class Interval(object):
    def __init__(self, lo, hi):
        self.lo = lo
        self.hi = hi
    #this makes it look prettier 
    def __repr__(self):
        return f'Interval({self.lo}, {self.hi})'

    @property
    def width(self):
        return self.hi - self.lo


class IntervalType(types.Type):
    #inherit constructor from types
    def __init__(self):
        super(IntervalType, self).__init__(name='Interval')

#to infer python objects as numba types
interval_type = IntervalType()
@typeof_impl.register(Interval)
def typeof_index(val, c):
    return interval_type

#used to infer numba types from python types
#intevral_type is are numba type 
as_numba_type.register(Interval, interval_type)

#this js teaches numba to recognize the constructor not implement it 
@type_callable(Interval)
def type_interval(context):
    def typer(lo, hi):
        #where we overload??????????
        if isinstance(lo, types.Float) and isinstance(hi, types.Float):
            return interval_type
    return typer


@register_model(IntervalType)
class IntervalModel(models.StructModel):
    def __init__(self, dmm, fe_type):
        members = [('lo', types.float64),
                   ('hi', types.float64),]
        models.StructModel.__init__(self, dmm, fe_type, members)

#make them accessible as .lo and .hi respectively
make_attribute_wrapper(IntervalType, 'lo', 'lo')
make_attribute_wrapper(IntervalType, 'hi', 'hi')

#we have to re implement not bc its a property but bc it is computeed rather than syred
#dont need type inference hook bc has a high level api combining type inferance and code gen
@overload_attribute(IntervalType, "width")
def get_width(interval):
    def getter(interval):
        return interval.hi - interval.lo
    return getter

@lower_builtin(Interval, types.Float, types.Float)
def impl_interval(context, builder, sig, args):
    typ = sig.return_type
    lo, hi = args
    interval = cgutils.create_struct_proxy(typ)(context, builder)
    interval.lo = lo
    interval.hi = hi
    #to get out the LLVm value from wrapper since llvm strcut values are quite low level and need
    #easier way to get and set members 
    return interval._getvalue()

#teach numba how to make it into native code to actually be able to compikle
@unbox(IntervalType)
def unbox_interval(typ, obj, c):
    is_error_ptr = cgutils.alloca_once_value(c.builder, cgutils.false_bit)
    interval = cgutils.create_struct_proxy(typ)(c.context, c.builder)

    with ExitStack() as stack:
        lo_obj = c.pyapi.object_getattr_string(obj, "lo")
        #check if null
        with cgutils.early_exit_if_null(c.builder, stack, lo_obj):
            c.builder.store(cgutils.true_bit, is_error_ptr)
        #would we need one for each type then 
        lo_native = c.unbox(types.float64, lo_obj)
        c.pyapi.decref(lo_obj)
        with cgutils.early_exit_if(c.builder, stack, lo_native.is_error):
            c.builder.store(cgutils.true_bit, is_error_ptr)

        #does getting the getattrstring give u smthn yk to look for in like the compiled code
        hi_obj = c.pyapi.object_getattr_string(obj, "hi")
        with cgutils.early_exit_if_null(c.builder, stack, hi_obj):
            c.builder.store(cgutils.true_bit, is_error_ptr)
        hi_native = c.unbox(types.float64, hi_obj)
        c.pyapi.decref(hi_obj)
        with cgutils.early_exit_if(c.builder, stack, hi_native.is_error):
            c.builder.store(cgutils.true_bit, is_error_ptr)

        interval.lo = lo_native.value
        interval.hi = hi_native.value

    return NativeValue(interval._getvalue(), is_error=c.builder.load(is_error_ptr))

@box(IntervalType)
def box_interval(typ, val, c):
    ret_ptr = cgutils.alloca_once(c.builder, c.pyapi.pyobj)
    fail_obj = c.pyapi.get_null_object()

    with ExitStack() as stack:
        interval = cgutils.create_struct_proxy(typ)(c.context, c.builder, value=val)
        #mapping it to interval.lo?????
        lo_obj = c.box(types.float64, interval.lo)
        #put thje fail_obj into ret_ptr if anything becomes null
        with cgutils.early_exit_if_null(c.builder, stack, lo_obj):
            c.builder.store(fail_obj, ret_ptr)

        hi_obj = c.box(types.float64, interval.hi)
        with cgutils.early_exit_if_null(c.builder, stack, hi_obj):
            c.pyapi.decref(lo_obj)
            c.builder.store(fail_obj, ret_ptr)

        class_obj = c.pyapi.unserialize(c.pyapi.serialize_object(Interval))
        with cgutils.early_exit_if_null(c.builder, stack, class_obj):
            c.pyapi.decref(lo_obj)
            c.pyapi.decref(hi_obj)
            c.builder.store(fail_obj, ret_ptr)
        res = c.pyapi.call_function_objargs(class_obj, (lo_obj, hi_obj))
        #take obj below outta mem
        c.pyapi.decref(lo_obj)
        c.pyapi.decref(hi_obj)
        c.pyapi.decref(class_obj)
        c.builder.store(res, ret_ptr)

    return c.builder.load(ret_ptr)


@njit 
def inside_interval(interval, x):
    return interval.lo <= x < interval.hi

@njit
def interval_width(interval):
    return interval.width


@njit
def sum_interval(i, j):
    return Interval(i.lo + j.lo, i.hi + j.hi)

i_int = Interval(2.0, 5.0)
j_int = Interval(3.0, 5.0)
k = sum_interval(i_int, j_int)
w = inside_interval(i_int, 4.0)
v = inside_interval(k, 4.0)
print(w)
print(v)


    












    