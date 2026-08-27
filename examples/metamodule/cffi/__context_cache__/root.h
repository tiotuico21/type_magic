#include <typeinfo>
#include "../../../../include/include.h"

#include <chrono>
#include <thread>

#include <functional>

#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define STRINGIFY_HELPER(x) #x
#define LINE_STRING STRINGIFY(__LINE__)
#ifdef COLOR_ASSERTS
#define ASSERT_TEXT(TEXT) "\n\n\e[33m\e[1m" __FILE__ ":" LINE_STRING " \e[0m\e[33m" TEXT "\e[0m\n"
#else
#define ASSERT_TEXT(TEXT) "\n\n" __FILE__ ":" LINE_STRING TEXT "\n"
#endif



#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

template <typename TYPE>
static std::string query(){
    return __PRETTY_FUNCTION__;
}

template <typename TYPE>
static std::string get_type_name(){
    std::string voidPRETTY = query<void>();
    std::string ourPRETTY = query<TYPE>();

    int typeIndex = voidPRETTY.find("void");

    int difference = ourPRETTY.length() - voidPRETTY.length();

    int totalLength = difference + 4;

    return ourPRETTY.substr(typeIndex, totalLength);

}


std::vector<std::string> extern_func_headers = {
	"def construct():",
	"def destruct(ptr):",
	"def add_one(ptr, arg1, arg2, arg3):",
	"def add_it(ptr):",
	"def sub_args(ptr, arg1, arg2, arg3):",
	"def is_true(ptr, arg1):"
};
std::vector<std::string> extern_func_param = {
	"()",
	"(ptr)",
	"(ptr, arg1, arg2, arg3)",
	"(ptr)",
	"(ptr, arg1, arg2, arg3)",
	"(ptr, arg1)",
	"(retptr, ptr, arg1, arg2, arg3)",
	"(retptr, ptr)",
	"(retptr, ptr, arg1, arg2, arg3)",
	"(retptr, ptr, arg1)"
};

std::string static cppToNumbaType(std::string cppType)
{
    static const std::unordered_map<std::string, std::string> typeMap = {
        {"bool", "types.boolean"},

        {"char", "types.int8"},
        {"signed char", "types.int8"},
        {"unsigned char", "types.uint8"},

        {"short", "types.int16"},
        {"unsigned short", "types.uint16"},

        {"int", "types.int32"},
        {"unsigned", "types.uint32"},
        {"unsigned int", "types.uint32"},

        {"long", "types.int64"},
        {"unsigned long", "types.uint64"},

        {"long long", "types.int64"},
        {"unsigned long long", "types.uint64"},

        {"int8_t", "types.int8"},
        {"uint8_t", "types.uint8"},
        {"int16_t", "types.int16"},
        {"uint16_t", "types.uint16"},
        {"int32_t", "types.int32"},
        {"uint32_t", "types.uint32"},
        {"int64_t", "types.int64"},
        {"uint64_t", "types.uint64"},

        {"float", "types.float32"},
        {"double", "types.float64"},
        {"std::string",
         "CPointer(types.int8)"},
        {"std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
         "CPointer(types.int8)"},
        {"void", "types.void"}};

    // Remove trailing whitespace
    while (!cppType.empty() && std::isspace(cppType.back()))
        cppType.pop_back();

    int pointerDepth = 0;

    // Count trailing '*'
    while (!cppType.empty() && cppType.back() == '*')
    {
        ++pointerDepth;
        cppType.pop_back();

        while (!cppType.empty() && std::isspace(cppType.back()))
            cppType.pop_back();
    }

    // Special case: void*
    if (cppType == "void" && pointerDepth == 1)
        return "types.voidptr";

    auto it = typeMap.find(cppType);

    if (it == typeMap.end())
        throw std::runtime_error("Unknown C++ type: " + cppType);

    std::string result = it->second;

    for (int i = 0; i < pointerDepth; ++i)
    {
        result = "types.CPointer(" + result + ")";
    }

    return result;
}



std::vector<std::string> extern_linker_headers = {};
std::vector<std::string> extern_meta_headers = {};
std::vector<std::string> meta_specialization = {};

template <auto... THING>
struct Fn
{
};

template <typename RES, typename CLASS, typename... ARGS, RES (CLASS::*X)(ARGS...)>
struct Fn<X>
{
	static constexpr auto value = X;
	typedef RES Result;
	typedef CLASS Class;
	typedef container::TypeArray<ARGS...> Args;
	typedef RES (CLASS::*type)(ARGS...);

	static auto call(CLASS *self, ARGS... args)
	{
		return (self->*X)(args...);
	}
};

template <typename TYPE>
struct IsFn
{
	static constexpr bool value = false;
};

template <auto ARG>
struct IsFn<Fn<ARG>>
{
	static constexpr bool value = true;
};


template <typename TRAIT>
struct FFIEntry
{
};

struct FFIGen
{
};


template <typename... ENTRIES>
struct StaticTable
{
	typedef container::TypeMap<ENTRIES...> EntriesTypeMap;
	static_assert(
		EntriesTypeMap::template FilterItems<container::util::Negate<IsFn>::template Template>::type::ITEM_COUNT == 0,
		ASSERT_TEXT("ERROR: No bindings that have items with the appropriate Fn format"));

	template <typename TRAIT, typename... ARGS>
	static auto call(ARGS... args)
	{
		typedef typename EntriesTypeMap::template ItemAt<TRAIT>::type FnPtr;
		return FnPtr::call(args...);
	}
};

struct SubOne{
    struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, float(void*, float)>>
	STable;
};

template <typename CONTEXT>
struct ImplSubOne{
	float fn(float arg1){
		return arg1 - 1.0f;
	}
	
typedef StaticTable<
        container::Binding<
SubOne::CallFn, Fn<&ImplSubOne<CONTEXT>::fn>>>
		STable;

};
using SubOneModule = context::SimpleModule<
	Meta<ImplSubOne>,
	context::RequirementSet<>,
	context::ImplementationSet<SubOne, FFIEntry<SubOne>>
>;





template <typename T>
struct Print{
    struct PrintFn{};

    typedef StaticTable<
        container::Binding<PrintFn, void(T)>>
        STable;
};


template <typename T>
struct PrintImplMeta{
    template <typename CONTEXT>
    struct PrintImpl{
        void my_print(T item){
            std::cout << "Print: " << item << std::endl;
        }
        typedef StaticTable<
            container::Binding<
                typename Print<T>::PrintFn,
                Fn<&PrintImplMeta::template PrintImpl<CONTEXT>::my_print>
            >
        >
        STable;
    };

    typedef context::SimpleModule <
        Meta<PrintImpl>,
        context::RequirementSet<>,
        context::ImplementationSet<Print<T>, FFIEntry<Print<T>>>
    > Module;
};

template <typename T>
struct PrintImplFFIMeta {
    typedef context::EmptyModule Module;
};

template <typename T>
struct PrintImplFFIMeta <Print<T>> { 
    typedef context::SimpleModule <
            Meta<PrintImplMeta<T>::template PrintImpl>,
            context::RequirementSet<>,
            context::ImplementationSet<Print<T>, FFIEntry<Print<T>>>
        > Module;
};


typedef context::ModuleBundle<
    context::MetaModule <
        Print,
        PrintImplMeta
    >,
    context::MetaModule <
        FFIEntry,
        PrintImplFFIMeta
    >
> PrintModule;



template <typename Class, typename... Types>
struct TypeList
{
};

template <typename T>
struct PureFnEq;

template <typename RES, typename CLASS, typename... ARGS>
struct PureFnEq<RES (CLASS::*)(ARGS...)>
{
	typedef RES type(CLASS *, ARGS...);
	typedef container::TypeArray<CLASS *, ARGS...> Args;
	typedef RES Result;
};


template <typename CONTEXT>
struct FFIGenImpl{

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


    template <typename T>
    void addFunctionGenRecurse(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {
        if constexpr (std::is_same<T, container::TypeSet<>>::value)
        {
            return;
        }
        // why did i have to guard this with an else
        else
        {
            typedef typename T::MapType::HeadItemType CurrTrait;
            //typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::string trait_name = get_type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCase(trait_name);

            typedef As<CurrTrait, CONTEXT> sig_component;

            std::string func_sig = get_type_name<CONTEXT>(); 
            if (isCpp)
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(true, trait_name, gen_file, python_file, is_for_CPU);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, isCpp, is_for_CPU);
            }
            else
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(false, trait_name, gen_file, python_file, is_for_CPU);
                addFunctionGenRecurse<typename T::MapType::TailType::KeySet>(gen_file, python_file, false, is_for_CPU);
            }
        }
    }

    // Helper: add indentation to every line AFTER the first line.
    // The first line stays where it was inserted.
    std::string indent_after_first_line(
        const std::string& str,
        int spaces
    )
    {
        std::string result = str;
        std::string indent(spaces, ' ');

        for (size_t i = 0; i < result.size(); ++i) {
            if (result[i] == '\n' && i + 1 < result.size()) {
                result.insert(i + 1, indent);

                // Skip over the indentation we just inserted
                i += spaces;
            }
        }

        return result;
    }


    static void write_extern_to_python_file_cpu(std::fstream &python_file,
                                    std::string typemagic_mangle_name,
                                    std::string extern_function_return_type,
                                    std::string extern_function_param_list){
        //std::cout << "Not print: " << get_type_name<KEY>().substr(0, 5)  << std::endl;
                    
        std::string extern_python_function = "extern_" + typemagic_mangle_name + " = nb.types.ExternalFunction(\n\t\""
                                + typemagic_mangle_name
                                + "\",\n\tnb.core.typing.signature(\n\t\t"
                                + extern_function_return_type + ", \n\t\t"
                                + extern_function_param_list
                                + "\n\t)\n)\n\n";

        python_file << extern_python_function;
        //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name));
        extern_linker_headers.push_back("extern_" + typemagic_mangle_name);

    }


    static void write_extern_to_python_file_gpu(std::fstream &python_file,
                                                std::string reg_str_func_name,
                                                std::string typemagic_mangle_name,
                                                std::string extern_function_return_type,
                                                std::string extern_function_param_list){
         python_file << "extern_" << toSnakeCase(reg_str_func_name) << "_gpu = cuda.declare_device(\n\t\""
                    << typemagic_mangle_name + "_gpu" << "\", \n\tnb.core.typing.signature("
                    << extern_function_return_type
                    << "(" << extern_function_param_list << ")))\n\n";
        //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name) + "_gpu");
        extern_linker_headers.push_back("extern_" + typemagic_mangle_name);
    }
    static void write_function_to_cpp_file_cpu(std::fstream &gen_file,
                                  std::string resultType,
                                  std::string typemagic_mangle_name,
                                  std::string header,
                                  std::string traitName,
                                  std::string reg_str_func_name,
                                  std::string arg_list){
        gen_file << "extern \"C\" "
                << resultType << " " << typemagic_mangle_name << "(" << header << ")"
                << "{"
                << std::endl
                << "\t"
                << get_type_name<CONTEXT>()
                << "* ptr = ("
                << get_type_name<CONTEXT>()
                << "*) arg0;"
                << std::endl
                << "\treturn As<"
                << traitName
                << ", "
                << get_type_name<CONTEXT>()
                << ">::STable::template call<typename "
                << reg_str_func_name
                << ">(&(as<"
                << traitName;
        if (arg_list.size() == 0)
        {
            gen_file << ">(*ptr)));";
        }
        else
        {
            gen_file << ">(*ptr)), "
                        << arg_list
                        << ");";
        }
        gen_file << std::endl
                    << "}"
                    << std::endl;
    
    }


    static void write_function_to_cpp_file_gpu(std::fstream &gen_file,
                                               std::string typemagic_mangle_name,
                                               std::string resultType,
                                               std::string header,
                                               std::string traitName,
                                               std::string reg_str_func_name,
                                               std::string arg_list){
        gen_file << "extern \"C\" "
                << "int" << " " << typemagic_mangle_name + "_gpu" << "(" << resultType + "* retptr, " + header << ")"
                << "{"
                << std::endl
                << "\t"
                << get_type_name<CONTEXT>()
                << "* ptr = ("
                << get_type_name<CONTEXT>()
                << "*) arg0;"
                << std::endl
                << "\t*retptr = As<"
                << traitName
                << ", "
                << get_type_name<CONTEXT>()
                << ">::STable::template call<typename "
                << reg_str_func_name
                << ">(&(as<"
                << traitName;
        if (arg_list.size() == 0)
        {
            gen_file << ">(*ptr)));";
        }
        else
        {
            gen_file << ">(*ptr)), "
                        << arg_list
                        << ");";
        }
        gen_file << std::endl
                    << "\treturn 1;"
                    << "\n}"
                    << std::endl;
    }
   
    static void write_header_to_cpp_file_cpu(std::fstream &gen_file,
                                             std::string resultType,
                                             std::string typemagic_mangle_name,
                                             std::string header)
    {
        gen_file << "extern \"C\" "
            << resultType
            << " "
            << typemagic_mangle_name
            << "("
            << header
            << ")"
            << ";"
            << std::endl;
    }

    static void write_header_to_cpp_file_gpu(std::fstream &gen_file,
                                             std::string resultType,
                                             std::string typemagic_mangle_name,
                                             std::string header)
    {
        gen_file << "extern \"C\" "
            << "int"
            << " "
            << typemagic_mangle_name + "_gpu"
            << "("
            << resultType + "* retptr, " + header
            << ")"
            << ";"
            << std::endl;
    }

    static std::string make_njit_param_list(int param_amount){
        std::string param_list = "ptr";
        if (param_amount == 0){
            return "ptr";
        }
        for (int i = 1; i <= param_amount; i++){
            std::string arg = "arg" + std::to_string(i);
            param_list += ", " + arg;
        }
        return param_list;
    }
   
    template <typename... T>
    struct ReadEveryFunction;

    template <typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<TAIL...>>
    {

        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {
            return;
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {
            std::string mangle_func_name = typeid(KEY).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string reg_str_func_name = get_type_name<KEY>();

            std::cout << "________________________REGULAR FUNC" << reg_str_func_name << std::endl;

            if (isCpp)
            {
                typedef typename ITEM::Args method_args_list;

                std::cout << "\n\nITEMMMMMMM COUNT" << std::endl;
                std::cout << method_args_list::MapType::ITEM_COUNT << std::endl;
                int param_amount = method_args_list::MapType::ITEM_COUNT;

                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            
                std::string extern_function_return_type = "nb." + cppToNumbaType(resultType);
                std::string extern_function_param_list = ParamListToString<outter_args_list>::makeString(0, true, true);


                if (is_for_CPU){
                    //python_file << "extern_" << toSnakeCase(reg_str_func_name) << " = numba.types.ExternalFunction(\n\t\""
                    
                  
                        /*
                        @nb.njit(cache=False)
                    def add_one(ptr, arg1, arg2, arg3):
                        return extern__TYPEMAGICNAddOne6CallFnE(ptr, arg1, arg2, arg3)

                        extern_linker_headers.push_back("extern_" + typemagic_mangle_name);

                          for (size_t i = 0; i < extern_func_headers.size(); ++i)
        {
            python_file << "@nb.njit(cache=False)\n";
            python_file << extern_func_headers[i] << "\n\t"
                        << "return " << extern_linker_headers[i] << extern_func_param[i] << "\n\n";
        }
                        */
                
                    write_extern_to_python_file_cpu(python_file,
                                                typemagic_mangle_name, 
                                                extern_function_return_type,
                                                extern_function_param_list);

                    write_function_to_cpp_file_cpu(gen_file,
                                               resultType,
                                               typemagic_mangle_name,
                                               header,
                                               traitName, 
                                               reg_str_func_name,
                                               arg_list);

                    python_file << "@nb.njit(cache=False)\n";
                    std::string njit_param_list = make_njit_param_list(param_amount);
                    python_file << "def " + toSnakeCase(traitName) + "(" + njit_param_list + "):\n\t"
                                << "return extern_" << typemagic_mangle_name
                                << "(" << njit_param_list + ")\n\n";

                


                    /*
                    @nb.njit(cache=False)
                    def add_one(ptr, arg1, arg2, arg3):
                        return extern__TYPEMAGICNAddOne6CallFnE(ptr, arg1, arg2, arg3)

                    */

                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{

                    write_extern_to_python_file_gpu(python_file,
                                                    reg_str_func_name,
                                                    typemagic_mangle_name, 
                                                    extern_function_return_type,
                                                    extern_function_param_list);
                    //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name) + "_gpu");
                   
                    write_function_to_cpp_file_gpu(gen_file,
                                                   typemagic_mangle_name,
                                                   resultType,
                                                   header,
                                                   traitName, 
                                                   reg_str_func_name,
                                                   arg_list);
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
            }
            else
            {
                typedef typename ITEM::Args method_args_list;
                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;

                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

                if (is_for_CPU){
                    write_header_to_cpp_file_cpu(gen_file,
                                                 resultType,
                                                 typemagic_mangle_name,
                                                 header);
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{
                    write_header_to_cpp_file_cpu(gen_file,
                                                 resultType,
                                                 typemagic_mangle_name,
                                                 header);
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                
                // ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType)
            }
        }
    };


    

    void addConstructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if ((extern_linker_headers.size() == 0 || extern_linker_headers[0] != "extern_construct") && isCpp)
        {
            if (is_for_CPU){
                python_file << "extern_construct = nb.types.ExternalFunction(\n\t\"construct\",\n\tnb.core.typing.signature(nb.types.voidptr)\n)\n\n";
                python_file << "@nb.njit(cache=False)\n"
                            << "def construct():\n\t"
                            << "return extern_construct()\n\n";               
            }
            else{
                python_file << "extern_construct_gpu = cuda.declare_device(\n\t\""
                            << "construct_gpu\", \n\tnb.core.typing.signature("
                            << "nb.types.voidptr"
                            << "()))\n\n";
                /*
                @nb.njit(cache=False)
                def construct():
                    return extern_construct()
                */
                python_file << "@nb.njit(cache=False)\n"
                            << "def construct():\n\t"
                            << "return extern_construct()\n\n";

            }
        }
 

        // container::repr::type_name<CONTEXT>()
        if (isCpp)
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void* construct()";
                gen_file << "{"
                         << std::endl 
                         << "\tvoid* myPtr = (void*) new "
                         << get_type_name<CONTEXT>()
                         << ";"
                         << std::endl
                         << "\tstd::cout << \"Constructor Ptr:\" << myPtr << std::endl;"
                         << std::endl
                         << "return myPtr;"
                         << "\n}"
                         << std::endl;
            }
            else{
                gen_file << "extern \"C\" int construct_gpu(void* retptr)";
                gen_file << "{"
                         << std::endl
                         << "\tretptr = (void*) new "
                         << get_type_name<CONTEXT>()
                         << ";"
                         << std::endl
                         << "\treturn 1;"
                         << "\n}"
                         << std::endl;
            }
        }
        else
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void* construct();" << std::endl;
            }
            else{
                gen_file << "extern \"C\" int construct_gpu(void* retptr);" << std::endl;
            }
        }
    }
     

    void addDestructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if ((extern_linker_headers.size() <= 1 || extern_linker_headers[1] != "extern_destruct") && isCpp)
        {
            if (is_for_CPU){
                python_file << "extern_destruct = nb.types.ExternalFunction(\n\t\"destructor\",\n\tnb.core.typing.signature(\n\t\tnb.types.voidptr, nb.types.voidptr\n\t)\n)\n\n";
                python_file << "@nb.njit(cache=False)\n"
                        << "def destruct(ptr):\n\t"
                        << "return extern_destruct(ptr)\n\n";          
            }
            else{
                python_file << "extern_destruct_gpu = cuda.declare_device(\n\t\""
                            << "destructor_gpu\", \n\tnb.core.typing.signature("
                            << "nb.types.void"
                            << "(nb.types.voidptr)))\n\n";
                python_file << "@nb.njit(cache=False)\n"
                    << "def destruct(ptr):\n\t"
                    << "return extern_destruct(ptr)\n\n";
            }
        }

        if (isCpp)
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void destructor(void* ptr)";
                gen_file << "{"
                         << std::endl
                         << get_type_name<CONTEXT>()
                         << "*ptr_to_delete = ("
                         << get_type_name<CONTEXT>()
                         << "*) ptr;"
                         << "\n\treturn delete ptr_to_delete;"
                                     
                         << "\n}\n\n";
            }
            else{
                //gen_file << "extern \"C\" __device__\n";
                gen_file << "extern \"C\" int destructor_gpu(void* retptr, void* ptr)";
                gen_file << "{"
                         << std::endl
                         << get_type_name<CONTEXT>()
                         << "*ptr_to_delete = ("
                         << get_type_name<CONTEXT>()
                         << "*) ptr;"
                         << "\n\tdelete ptr_to_delete;"
                         << "\n\treturn 1;"
                         << "\n}\n";
            }
        }
        else
        {
            if (is_for_CPU){
               gen_file << "extern \"C\" void destructor(void* ptr);" << std::endl;
            }
            else{
               gen_file << "extern \"C\" int destructor_gpu(void* retptr, void* ptr);" << std::endl;
            } 
        } 
    }


     static std::string toSnakeCase(std::string &trait_name)
    {
        std::string snake_case_trait_name;

        for (size_t i = 0; i < trait_name.size(); i++)
        {
            if (std::isupper(static_cast<unsigned char>(trait_name[i])) || static_cast<unsigned char>(trait_name[i]) == ':')
            {
                if (i != 0)
                {
                    snake_case_trait_name += '_';
                }
                if (static_cast<unsigned char>(trait_name[i]) != ':')
                {
                    snake_case_trait_name += std::tolower(static_cast<unsigned char>(trait_name[i]));
                }
            }
            else
            {
                snake_case_trait_name += trait_name[i];
            }
        }
        return snake_case_trait_name;
    }


    static std::string toSnakeCaseMeta(const std::string& trait_name)
    {
        size_t start = trait_name.find('<');
        size_t end = trait_name.rfind('>');

        // No <...> found: just snake-case the whole name
        if (start == std::string::npos ||
            end == std::string::npos ||
            end <= start)
        {
            return trait_name;
        }

        // Get "Meta" and "Print"
        std::string prefix = trait_name.substr(0, start);
        std::string inner  = trait_name.substr(start + 1, end - start - 1);

        std::string result;

        // Add prefix: Meta -> meta
        for (char c : prefix)
        {
            result += static_cast<char>(
                std::tolower(static_cast<unsigned char>(c))
            );
        }

        // Add underscore between Meta and Print
        result += '_';

        // Convert inner to snake_case
        for (size_t i = 0; i < inner.size(); ++i)
        {
            unsigned char c = static_cast<unsigned char>(inner[i]);

            if (std::isupper(c))
            {
                // Add _ before uppercase letters, except the first character
                if (i != 0 && inner[i - 1] != '_')
                {
                    result += '_';
                }

                result += static_cast<char>(std::tolower(c));
            }
            else
            {
                result += inner[i];
            }
        }

        return result;
    }
    template <typename T>
    struct ParamListToString;

    // template <typename... ARGS>
    template <typename... TAIL>
    struct ParamListToString<container::TypeArray<TAIL...>>
    {
        static std::string makeString(int index, bool includeType, bool forPython = false)
        {
            return "";
        }
    };

    template <typename HEAD, typename... TAIL>
    struct ParamListToString<container::TypeArray<HEAD, TAIL...>>
    {
        static std::string makeString(int index, bool includeType, bool forPython = false)
        {
            // std::cout << "i am going into the recursive case" << std::endl;
            std::string str_head_type = get_type_name<HEAD>();

            std::string str_types_from_tail = ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType, forPython);

            // std::cout << "index: " << index << "-> " << str_head_type << std::endl;
            std::string total_param_list = "";
            if (forPython)
            {
                std::string current =
                    (index == 0)
                        ? "nb." + cppToNumbaType("void*")
                        : "nb." + cppToNumbaType(str_head_type);

                if (str_types_from_tail.empty())
                    return current;

                return current + ", " + str_types_from_tail;
            }
            if (includeType)
            {
                if (index == 0)
                {
                    total_param_list += "void*";
                }
                else
                {
                    total_param_list += get_type_name<HEAD>();
                }
            }
            if (container::TypeArray<TAIL...>::MapType::ITEM_COUNT == 0)
            {
                if (includeType)
                {
                    total_param_list += " arg" + std::to_string(index);
                }
                else
                {
                    total_param_list += "arg" + std::to_string(index);
                }
            }
            else
            {
                if (includeType)
                {
                    total_param_list += " arg" + std::to_string(index) + ", " + str_types_from_tail;
                }
                else
                {
                    total_param_list += "arg" + std::to_string(index) + ", " + str_types_from_tail;
                }
            }
            // std::cout << "toal param inside tostring recursive: " << total_param_list << std::endl;
            return total_param_list;
        }
    };


    int findSpace(std::string func_sig)
    {
        bool entered = false;
        int depth = 0;
        for (int i = func_sig.length(); i >= 0; i--)
        {

            if (func_sig[i] == ')' && !entered)
            {
                depth = 1;
                entered = true;
            }
            else if (func_sig[i] == '(' && entered)
            {
                depth--;
                if (depth == 0)
                {
                    return i - 1;
                }
            }

            else if (func_sig[i] == ')' && entered)
            {
                depth++;
            }
        }
        return -1;
    }


    template <typename T>
    struct IsSpecialization{
        static constexpr bool value = false;
    };
    
    template <template<typename...>typename TEMPLATE, typename T>
    struct IsSpecialization<TEMPLATE<T>>{
        static constexpr bool value = true;
    };
    template <typename T>
    struct GetTemplate{
        typedef T type;
    };

    template <template <typename...>typename TEMPLATE, typename T>
    struct GetTemplate<TEMPLATE<T>>{
        typedef Meta<TEMPLATE> type;
    };

    template <typename TRAITMAP>
    struct FFIDetector {
        template <typename TRAIT>
        struct HasFFI{
            static constexpr bool value = TRAITMAP::template has_key<TRAIT>() && TRAITMAP::template has_key<FFIEntry<TRAIT>>();
        };
    };


    /*
    template <typename T>
void put_in_dict_file(std::fstream& dict_file){
    using namespace container;
    using namespace context;

    if constexpr (std::is_same<T, container::TypeSet<>>::value)
    {
        return;
    }
    else{
        typedef typename T::MapType::HeadItemType CurrTrait;

        if (IsMeta<CurrTrait>::value){
            std::string FullTrait = container::repr::type_name<CurrTrait>();
            size_t start = FullTrait.find('<');
            size_t end = FullTrait.find('>', start); 

            std::string InnerTrait = FullTrait.substr(start + 1, end - start -1);
            
            dict_file << "def " << InnerTrait << "(*args):";
            dict_file << "\n\treturn {cpp_name: \"" << InnerTrait
                      << "<\"+\",\".join(*args)+\">\"}\n";
            put_in_dict_file<typename T::MapType::TailType::KeySet>(dict_file);
        }
        else{
            typedef typename T::MapType::HeadItemType CurrTrait;
            dict_file << container::repr::type_name<CurrTrait>();
            dict_file << " = { cpp_name: \"" << container::repr::type_name<CurrTrait>() << "\"}\n\n";
            put_in_dict_file<typename T::MapType::TailType::KeySet>(dict_file);
        }
         
    }
}
    */
    //CFFIMeta::KeySet::template FIler<M::template Generalizes>::type my_meta_set


    /*
      std::cout << "*************KEY: " << reg_str_func_name.substr(0, 5)  << std::endl;
                        size_t start = reg_str_func_name.find('<');
                        size_t end = reg_str_func_name.find('>', start);

                        std::string cpp_print_type = reg_str_func_name.substr(start + 1, end - start - 1);
                        
                        std::string numba_type = "nb." + cppToNumbaType(cpp_print_type);
                        meta_specialization.push_back(numba_type);
                        std::string extern_python_function =
                        "nb.types.ExternalFunction(\n"
                        "    \"" + typemagic_mangle_name + "\",\n"
                        "    nb.core.typing.signature(\n"
                        "        " + extern_function_return_type + ",\n"
                        "        " + extern_function_param_list + "\n"
                        "    )\n"
                        ")";

                      
                        extern_meta_headers.push_back(extern_python_function);
    */

    /*
    
    template <typename... T>
    struct ReadEveryFunction;

    template <typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<TAIL...>>
    {

        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {
            return;
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {

         typedef typename T::MapType::HeadItemType CurrFFISpec;
            typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::string trait_name = get_type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCase(trait_name);

            typedef As<CurrTrait, CONTEXT> sig_component;

            std::string func_sig = get_type_name<CONTEXT>(); 
            if (isCpp)
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(true, trait_n
    */
    template <typename T>
    void handle_every_generic_trait(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU){
        if constexpr(std::is_same<T, container::TypeSet<>>::value){
            return;
        }
        else{
            typedef typename T::MapType::HeadItemType CurrTrait; 
            std::cout << "\n\nHEad: " << container::repr::type_name<CurrTrait>() << "/n/n" << std::endl;
         

            typedef typename  CONTEXT::TraitMap::KeySet::template Filter<CurrTrait::template Generalizes>::type MyMetaSet;
            std::string meta_trait_name = get_type_name<CurrTrait>();
            std::string meta_camel_name = toSnakeCaseMeta(meta_trait_name);
            std::cout << meta_camel_name << std::endl;
            std::cout << "METAAAAAAAAA SET:" << container::repr::type_name<MyMetaSet>() << "/n/n" << std::endl;
            
            make_extern_map_for_single_meta_trait<MyMetaSet>(gen_file, python_file, meta_camel_name, isCpp, is_for_CPU);
            handle_every_generic_trait<typename T::MapType::TailType::KeySet>(gen_file, python_file, isCpp, is_for_CPU);

        }
    }

    //traverse the specializations of Meta<print>
    template <typename CURRSET>
    void make_extern_map_for_single_meta_trait(std::fstream &gen_file, std::fstream &python_file, std::string meta_name, bool isCpp, bool is_for_CPU){
        std::string extern_map = meta_name + "_ext_map = {";
        std::string all_specialization_entries = make_all_meta_map_entries<CURRSET>(gen_file, python_file, meta_name, isCpp, is_for_CPU);
        std::cout << "\n\nSpecializerssss: " << all_specialization_entries << std::endl;


        std::string complete_extern_map_for_single_trait =
            meta_name + "_ext_map = {" +
            indent(all_specialization_entries) +
            "\n}\n";
        
        std::cout << complete_extern_map_for_single_trait << std::endl;
        python_file << complete_extern_map_for_single_trait;
    }

    template <typename CURRSET>
    static std::string make_all_meta_map_entries(std::fstream &gen_file, std::fstream &python_file, std::string meta_name, bool isCpp, bool is_for_CPU){
        if constexpr(std::is_same<CURRSET, container::TypeSet<>>::value){
            return "";
        }
        else{
            std::string result = "";
            typedef typename CURRSET::MapType::HeadItemType CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::string trait_name = get_type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCaseMeta(trait_name);

            std::cout << "\n\nTRAITSSSS " << trait_name_snake_case << std::endl;

            typedef As<CurrTrait, CONTEXT> sig_component;
            std::string single_entry = ReadMetaFunction<typename sig_component::STable::EntriesTypeMap>::exec(isCpp, trait_name, gen_file, python_file, is_for_CPU, "");
            std::cout << "single_entry: " << single_entry << std::endl;
            result += single_entry;
            result += make_all_meta_map_entries<typename CURRSET::MapType::TailType::KeySet>(gen_file, python_file, meta_name, isCpp, is_for_CPU);
            return result;
        }
    }

    static std::string meta_single_entry_generator(std::string typemagic_mangle_name,
                                            std::string extern_function_return_type,
                                            std::string extern_function_param_list,
                                            std::string reg_str_func_name,
                                            std::string traitName){
        std::string map_entry = "";
        size_t start = reg_str_func_name.find('<');
        size_t end = reg_str_func_name.find('>', start);

        std::string cpp_print_type = reg_str_func_name.substr(start + 1, end - start - 1);
        
        std::string numba_type = "nb." + cppToNumbaType(cpp_print_type);
        //meta_specialization.push_back(numba_type);
        std::string meta_handle = "my_" + traitName + "_type(" + numba_type + "):";
        map_entry = std::format(R"PY(
my_print_type({}): nb.types.ExternalFunction(
    "{}",
    nb.core.typing.signature(
        {},
        {}
    )
),)PY",
        numba_type,
        typemagic_mangle_name,
        extern_function_return_type,
        extern_function_param_list
        );
        return map_entry;
    }

      

    template <typename... T>
    struct ReadMetaFunction;

    template <typename... TAIL>
    struct ReadMetaFunction<container::TypeMap<TAIL...>>
    {
        static std::string exec(bool isCpp, 
                           std::string traitName, 
                           std::fstream &gen_file, 
                           std::fstream &python_file, 
                           bool is_for_CPU, 
                           std::string result,
                           int func_index = 0)
        {
            return "";
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadMetaFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
         static std::string exec(bool isCpp, 
                           std::string traitName, 
                           std::fstream &gen_file, 
                           std::fstream &python_file, 
                           bool is_for_CPU, 
                           std::string result,
                           int func_index = 0)
        {
            std::string mangle_func_name = typeid(KEY).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string reg_str_func_name = get_type_name<KEY>();

            std::cout << "88888888888MEta FUNC" << reg_str_func_name << std::endl;

            std::string map_entry = "";
            if (isCpp){
                typedef typename ITEM::Args method_args_list;

                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            
                std::string extern_function_return_type = "nb." + cppToNumbaType(resultType);
                std::string extern_function_param_list = ParamListToString<outter_args_list>::makeString(0, true, true);


                map_entry = meta_single_entry_generator(typemagic_mangle_name,
                                            extern_function_return_type,
                                            extern_function_param_list,
                                            reg_str_func_name,
                                            traitName);
            }
            return map_entry + ReadMetaFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, result, func_index + 1);
        }
    };

    static std::string indent(
    const std::string& text,
    const std::string& prefix = "    ")
    {
        std::string result;
        result.reserve(text.size() + 64);

        bool beginning_of_line = true;

        for (char c : text)
        {
            if (beginning_of_line && c != '\n')
            {
                result += prefix;
                beginning_of_line = false;
            }

            result += c;

            if (c == '\n')
                beginning_of_line = true;
        }

        return result;
    }

    /*
        {

    /*
    template <typename CURRSET>
    std::string make_extern_map_entries(std::string meta_name){
        if constexpr(std::is_same<T, container::TypeSet<>>::value){
            return;
        }
        else{
            typedef typename T::MapType::HeadItemType CurrSpecialization; 
            std::string mangle_func_name = typeid(CurrSpecialization).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string extern_python_function =
                    "nb.types.ExternalFunction(\n"
                    "    \"" + typemagic_mangle_name + "\",\n"
                    "    nb.core.typing.signature(\n"
                    "        " + extern_function_return_type + ",\n"
                    "        " + extern_function_param_list + "\n"
                    "    )\n"
                    ")";


        }
    }
    */
        
    void addFunctionHeaders(std::fstream &header_file, std::fstream &python_file, std::string client_header, bool is_for_CPU)
    {
        header_file << "#include <thread>"
                    << std::endl
                    << "#include \"../../../../include/include.h\""
                    << std::endl
                    << "#include <functional>"
                    << std::endl
                    << "#include <iostream>"
                    << std::endl
                    << "#include <fstream>"
                    << std::endl
                    << "#include \"" << client_header << "\""
                    << std::endl;

        addConstructor(header_file, python_file, false, is_for_CPU);
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFISet; // get every FFI specialization
        addFunctionGenRecurse<FFISet>(header_file, python_file, false, is_for_CPU);
        addDestructor(header_file, python_file, false, is_for_CPU);
    }

    void addFunctionBody(std::fstream &cpp_file, std::fstream &python_file, bool is_for_CPU)
    {
        //std::cout << "adding function body" << std::endl;
        cpp_file << "#include \"cffi.h\""
                 << std::endl
                 << std::endl;
        addConstructor(cpp_file, python_file, true, is_for_CPU);
    
       //filter will put every trait into isspecialization
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<IsSpecialization>::type MySet;
        std::cout << "THIS IS A SPECIALIZE TRAIT: " << container::repr::type_name<MySet>() << std::endl;
        //typedef typename MySet::template LossyMap<GetTemplate>::type MyMetaSet
        std::cout << "THIS IS ALL MY META TRAITS: " << container::repr::type_name<MySet>() << std::endl;
        typedef typename MySet::template Filter<FFIDetector<typename CONTEXT::TraitMap>::template HasFFI>::type FFISetUnMeta;
        typedef typename FFISetUnMeta::template LossyMap<GetTemplate>::type FFIMetaSet;
        //handle_every_generic_trait<MyMetaSet>(python_file);
        std::cout << "\n\nTRAITS THAT IMPLEMENT FFI" << container::repr::type_name<FFIMetaSet>() << std::endl;
        handle_every_generic_trait<FFIMetaSet>(cpp_file, python_file, true, is_for_CPU);

    
        std::cout << "\n\n Every non generic trait" << std::endl;
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<container::util::Negate<IsSpecialization>::template Template>::type NonGenSet;
        std::cout << container::repr::type_name<NonGenSet>() << std::endl;

        typedef typename NonGenSet::template Filter<FFIDetector<typename CONTEXT::TraitMap>::template HasFFI>::type FFINonGenSet;
        std::cout << "\n\n Every non generic trait that has FFI" << std::endl;
        std::cout << container::repr::type_name<FFINonGenSet>() << std::endl;

        //typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet;
        std::cout << "\n\n Every non generic trait" << std::endl;
        std::cout << container::repr::type_name<FFICppSet>() << std::endl;
        
        addFunctionGenRecurse<FFINonGenSet>(cpp_file, python_file, true, is_for_CPU);
        addDestructor(cpp_file, python_file, true, is_for_CPU);
    }


};


using FFIGenModule = context::SimpleModule<
    Meta<FFIGenImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<FFIGen>>;
    