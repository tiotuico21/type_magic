#include <iostream>

std::string meta_code = R"PY(

@overload_method(My__TYPE__Type, '__call__')
def call_overload_ffi(self, ctx, val):
    extern_fn = __NAME___ext_map[self]

    def impl(self, ctx, val):
        return extern_fn(ctx, val)

    return impl


@lower_builtin(My__TYPE__Type, My__TYPE__Type, types.VarArg(types.Any))
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


@typeof_impl.register(My__TYPE__)
def typeof_index(val, c):
    return __NAME___type(val.kind)


@type_callable(My__TYPE__)
def type___NAME__(context):

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

            print(__NAME___type(kind))

            print("________________________")
            print("")

            return __NAME___type(kind)

        else:
            raise NumbaTypeError(
                f"Type {kind} not in type set"
            )

    return typer


@register_model(My__TYPE__Type)
class My__TYPE__Model(models.StructModel):

    def __init__(self, dmm, fe_type):
        members = []

        models.StructModel.__init__(
            self,
            dmm,
            fe_type,
            members
        )


make_attribute_wrapper(
    My__TYPE__Type,
    'kind',
    'kind'
)


@lower_builtin(My__TYPE__, nb.types.Any)
def impl___NAME__(context, builder, sig, args):

    typ = sig.return_type

    __NAME__ = cgutils.create_struct_proxy(typ)(
        context,
        builder
    )

    return __NAME__._getvalue()


@unbox(My__TYPE__Type)
def unbox___NAME__(typ, obj, c):

    is_error_ptr = cgutils.alloca_once_value(
        c.builder,
        cgutils.false_bit
    )

    __NAME__ = cgutils.create_struct_proxy(typ)(
        c.context,
        c.builder
    )

    return NativeValue(
        __NAME__._getvalue(),
        is_error=c.builder.load(is_error_ptr)
    )


@box(My__TYPE__Type)
def box___NAME__(typ, val, c):

    print(f"type from boxing: {typ}")

    ret_ptr = cgutils.alloca_once(
        c.builder,
        c.pyapi.pyobj
    )

    fail_obj = c.pyapi.get_null_object()

    with ExitStack() as stack:

        class_obj = c.pyapi.unserialize(
            c.pyapi.serialize_object(My__TYPE__)
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
    retInstance = My__TYPE__(x)
    return retInstance


@nb.njit
def test___NAME__(___NAME___instance, ctx, x):
    ___NAME___instance(ctx, x)
)PY";