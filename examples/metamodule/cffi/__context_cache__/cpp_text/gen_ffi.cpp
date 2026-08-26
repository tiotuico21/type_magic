
    void genffi(std::string client_logic_header_file, bool is_for_CPU)
    {
        std::fstream header_file;
        std::fstream cpp_file;
        std::fstream python_file;
        header_file.open("cffi.h", std::ios::trunc | std::ios::out);
        cpp_file.open("cffi.cpp", std::ios::trunc | std::ios::out);
        python_file.open("test_ffi.py", std::ios::trunc | std::ios::out);
        std::string meta_code_for_numba = R"(
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
        )";
        python_file << meta_code_for_numba << "\n";
        python_file << "import re\n"
                    << "import sys\n"
                    << "import inspect\n"
                    << "import subprocess\n"
                    << "\n"
                    << "from llvmlite import binding\n"
                    << "\n"
                    << "binding.load_library_permanently(\"./my_dynamic_library.so\")\n\n\n";

        std::string meta_code_declaring_types = R"PY(
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
        )PY";

        python_file << meta_code_declaring_types << "\n";

        
        addFunctionHeaders(header_file, python_file, client_logic_header_file, is_for_CPU);
        addFunctionBody(cpp_file, python_file, is_for_CPU);
        header_file.close();
        cpp_file.close();

        /*
        for (size_t i = 0; i < extern_func_headers.size(); ++i)
        {
            python_file << "@nb.njit(cache=False)\n";
            python_file << extern_func_headers[i] << "\n\t"
                        << "return " << extern_linker_headers[i] << extern_func_param[i] << "\n\n";
        }
        

        std::string map_contents = "my_print_ext_map = {\n";

        for (size_t k = 0; k < extern_meta_headers.size(); ++k)
        {
            map_contents +=
                "    my_print_type(" +
                meta_specialization[k] +
                "): " +

                // Indent every line AFTER the first line
                indent_after_first_line(
                    extern_meta_headers[k],
                    4
                ) +

                ",\n";
        }

        map_contents += "}\n\n";

        python_file << map_contents;
        */
        std::string meta_code_overload = R"PY(
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
        )PY";

        python_file << meta_code_overload << "\n";

        std::string meta_code_for_processing_types = R"PY(
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
        
        
        )PY";

        python_file << meta_code_for_processing_types << "\n";


        /*
        def print_int(ptr, arg1):
	        return extern__TYPEMAGICN5PrintIiE7PrintFnE(ptr, arg1)
        */
        std::cout << python_file.is_open() << '\n';
        python_file.close();

        // why fPIC smthn about address reolacation
        

       
    }

