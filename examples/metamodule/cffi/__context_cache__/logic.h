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



template <typename T>
struct Print{
    struct PrintFn{};

    typedef StaticTable<
        container::Binding<PrintFn, void(T)>>
        STable;
};



struct AddOne{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, float(void*, float, bool, float)>>
	STable;
};
struct AddIt{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int(void*)>>
	STable;
};
struct SubArgs{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, float(void*, float, bool, float)>>
	STable;
};
struct IsTrue{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, bool(void*, bool)>>
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




extern "C" float add_one(void* ctx, float arg1, bool arg2, float arg3);
template <typename CONTEXT>
struct ImplAddOne{
	float call(float arg1, bool arg2, float arg3){
		float result;
		return add_one((CONTEXT*)this,arg1, arg2, arg3);
	}
	
typedef StaticTable<
        container::Binding<
AddOne::CallFn, Fn<&ImplAddOne<CONTEXT>::call>>>
		STable;

};
using AddOneModule = context::SimpleModule<
	Meta<ImplAddOne>,
	context::RequirementSet<>,
	context::ImplementationSet<AddOne, FFIEntry<AddOne>>
>;


extern "C" int add_it(void* ctx);
template <typename CONTEXT>
struct ImplAddIt{
	int call(){
		int result;
		return add_it((CONTEXT*)this);
	}
	
typedef StaticTable<
        container::Binding<
AddIt::CallFn, Fn<&ImplAddIt<CONTEXT>::call>>>
		STable;

};
using AddItModule = context::SimpleModule<
	Meta<ImplAddIt>,
	context::RequirementSet<>,
	context::ImplementationSet<AddIt, FFIEntry<AddIt>>
>;


extern "C" float sub_args(void* ctx, float arg1, bool arg2, float arg3);
template <typename CONTEXT>
struct ImplSubArgs{
	float call(float arg1, bool arg2, float arg3){
		float result;
		return sub_args((CONTEXT*)this,arg1, arg2, arg3);
	}
	
typedef StaticTable<
        container::Binding<
SubArgs::CallFn, Fn<&ImplSubArgs<CONTEXT>::call>>>
		STable;

};
using SubArgsModule = context::SimpleModule<
	Meta<ImplSubArgs>,
	context::RequirementSet<>,
	context::ImplementationSet<SubArgs, FFIEntry<SubArgs>>
>;


extern "C" bool is_true(void* ctx, bool arg1);
template <typename CONTEXT>
struct ImplIsTrue{
	bool call(bool arg1){
		bool result;
		return is_true((CONTEXT*)this,arg1);
	}
	
typedef StaticTable<
        container::Binding<
IsTrue::CallFn, Fn<&ImplIsTrue<CONTEXT>::call>>>
		STable;

};
using IsTrueModule = context::SimpleModule<
	Meta<ImplIsTrue>,
	context::RequirementSet<>,
	context::ImplementationSet<IsTrue, FFIEntry<IsTrue>>
>;



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

        std::string meta_code_overload = R"PY(
@overload_method(MyPrintType, '__call__')
def call_overload_2_arg(self, val):
    return my_print_ext_map[self]
        )PY";

        python_file << meta_code_overload << "\n";

        std::string meta_code_for_processing_types = R"PY(
@lower_builtin(MyPrintType, MyPrintType, types.VarArg(types.Any))
def method_impl(context, builder, sig, args):
    print("METHOD IMPLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL")
    typing_context = context.typing_context
    overload = call_overload_2_arg
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
    valid_type_set = set([nb.types.Integer,nb.types.Float, nb.types.Boolean,nb.types.functions.NumberClass, nb.types.int64, nb.types.float64,record_type, nb.types.void])
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

        addFunctionHeaders(header_file, python_file, client_logic_header_file, is_for_CPU);
        addFunctionBody(cpp_file, python_file, is_for_CPU);
        header_file.close();
        cpp_file.close();

     
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

        /*
        def print_int(ptr, arg1):
	        return extern__TYPEMAGICN5PrintIiE7PrintFnE(ptr, arg1)
        */
        python_file << "@nb.njit(cache=False)\n";
        python_file << "def print_int(ptr, arg1):\n\treturn extern__TYPEMAGICN5PrintIiE7PrintFnE(ptr, arg1)\n\n";
       
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
                    if (reg_str_func_name.substr(0, 5) == "Print"){
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
                        std::cout << "\n\n\nNUMBA TYPE: " << numba_type << std::endl;

                    }
                    else{
                        std::cout << "Not print: " << get_type_name<KEY>().substr(0, 5)  << std::endl;
                        
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
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{
                    python_file << "extern_" << toSnakeCase(reg_str_func_name) << "_gpu = cuda.declare_device(\n\t\""
                                             << typemagic_mangle_name + "_gpu" << "\", \n\tnb.core.typing.signature("
                                             << extern_function_return_type
                                             << "(" << extern_function_param_list << ")))\n\n";
                    //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name) + "_gpu");
                    extern_linker_headers.push_back("extern_" + typemagic_mangle_name);
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
                    gen_file << "extern \"C\" "
                             << resultType
                             << " "
                             << typemagic_mangle_name
                             << "("
                             << header
                             << ")"
                             << ";"
                             << std::endl;
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{
                   gen_file << "extern \"C\" "
                            << "int"
                            << " "
                            << typemagic_mangle_name + "_gpu"
                            << "("
                            << resultType + "* retptr, " + header
                            << ")"
                            << ";"
                            << std::endl;
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                
                // ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType)
            }
        }
    };


    

    void addConstructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if (extern_linker_headers.size() == 0 || extern_linker_headers[0] != "extern_construct")
        {
            if (is_for_CPU){
                python_file << "extern_construct = nb.types.ExternalFunction(\n\t\"construct\",\n\tnb.core.typing.signature(nb.types.voidptr)\n)\n\n";
                extern_linker_headers.push_back("extern_construct");               
            }
            else{
                 python_file << "extern_construct_gpu = cuda.declare_device(\n\t\""
                             << "construct_gpu\", \n\tnb.core.typing.signature("
                             << "nb.types.voidptr"
                             << "()))\n\n";
                 extern_linker_headers.push_back("extern_construct_gpu");
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

        if (extern_linker_headers.size() <= 1 || extern_linker_headers[1] != "extern_destruct")
        {
            if (is_for_CPU){
                python_file << "extern_destruct = nb.types.ExternalFunction(\n\t\"destructor\",\n\tnb.core.typing.signature(\n\t\tnb.types.voidptr, nb.types.voidptr\n\t)\n)\n\n";
                extern_linker_headers.push_back("extern_destruct");
            }
            else{
                python_file << "extern_destruct_gpu = cuda.declare_device(\n\t\""
                            << "destructor_gpu\", \n\tnb.core.typing.signature("
                            << "nb.types.void"
                            << "(nb.types.voidptr)))\n\n";
                extern_linker_headers.push_back("extern_destruct_gpu");
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
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<Print>::template Generalizes>::type PrintSet;
        std::cout << "\n\n\n\n****************The set of publically-advertised traits is: "
                << container::repr::type_name<PrintSet>()
                << std::endl;
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        addFunctionGenRecurse<FFICppSet>(cpp_file, python_file, true, is_for_CPU);
        addDestructor(cpp_file, python_file, true, is_for_CPU);
    }


};


using FFIGenModule = context::SimpleModule<
    Meta<FFIGenImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<FFIGen>>;
    