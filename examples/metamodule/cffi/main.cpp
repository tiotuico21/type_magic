#include <thread>
#include "../../../include/include.h"
#include <functional>
#include <iostream>
#include <fstream>

struct TraitA
{
};

struct TraitB
{
};

template <typename TRAIT>
struct FFIEntry
{
};

struct FFIGen
{
};

template <typename CONTEXT>
struct BImpl
{
    float fn(double x)
    {
        float returnVal = x * 3.0;
        return returnVal;
    }
};

using BModule = context::SimpleModule<
    Meta<BImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<TraitB, FFIEntry<TraitB>>>;

template <typename CONTEXT>
struct AImpl
{
    void fn(int x, bool y)
    {
        if (y)
        {
            std::cout << x << std::endl;
        }
        std::cout << "false" << std::endl;
    }
};

using AModule = context::SimpleModule<
    Meta<AImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<TraitA, FFIEntry<TraitA>>>;

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
    // do i make a new template struct js fir the type list
    // why do we havw to iterate thru the class *
    // what kinda type is res

    // am i even declaring a type here if all i want is a Type Array hold the arguments which are params
    typedef container::TypeArray<CLASS *, ARGS...> Args;
    typedef RES Result;
};

// how does it work when you add two type def to this struct bc wwhen you pass in a param what do u do to return the one u want

template <typename CONTEXT>
struct FFIGenImpl
{
    void genffi()
    {
        std::fstream header_file;
        std::fstream cpp_file;
        header_file.open("cffi.h", std::ios::trunc | std::ios::out);
        cpp_file.open("cffi.cpp", std::ios::trunc | std::ios::out);
        addFunctionHeaders(header_file);
        addFunctionBody(cpp_file);
        header_file.close();
    }

    template <typename T>
    void addFunctionHeaderRecurse(std::fstream &header_file)
    {
        if constexpr (std::is_same<T, container::TypeSet<>>::value)
        {
            std::cout << "all empty" << std::endl;
            return;
        }
        // header_file.open(file_name, std::ios::trunc | std::ios::out); idt i need bc then we would clear/go back to the beginning
        else
        {

            typedef typename T::MapType::HeadItemType CurrFFISpec;
            typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string trait_name = container::repr::type_name<CurrTrait>(); // typeid(CurrTrait).name()

            typedef As<CurrTrait, CONTEXT> sig_component;
            typedef decltype(&sig_component::fn) method_pointer_sig;
            typedef typename PureFnEq<method_pointer_sig>::type pure_func_sig;
            std::string func_sig = container::repr::type_name<pure_func_sig>(); //

            int indexForName = findSpace(func_sig);
            if (indexForName == -1)
            {
                return;
            }

            std::string header = func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);
            std::cout << header << std::endl;
            // should i put it into a char pointer then
            header_file << header << std::endl;

            addFunctionHeaderRecurse<typename T::MapType::TailType::KeySet>(header_file);
        }
    }

    template <typename T>
    void addFunctionBodyRecurse(std::fstream &cpp_file)
    {
        if constexpr (std::is_same<T, container::TypeSet<>>::value)
        {
            std::cout << "all empty" << std::endl;
            return;
        }
        else
        {
            // function name
            typedef typename T::MapType::HeadItemType CurrFFISpec;
            typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string trait_name = container::repr::type_name<CurrTrait>();

            // function sig
            typedef As<CurrTrait, CONTEXT> sig_component;

            // how does method sig of purefneq-> bc thats the format the function in
            typedef decltype(&sig_component::fn) method_pointer_sig;
            typedef typename PureFnEq<method_pointer_sig>::type pure_func_sig;
            std::string func_sig = container::repr::type_name<pure_func_sig>(); //

            std::cout << "func sig -> " << func_sig << std::endl;

            // func param list
            // get the args list

            // iterate through the classes to because each can have their own param list

            // should prob put stuff in a helper method

            // i want to input the method pointer sig and reach in to get the args type def to make a type list i can iterate over
            typedef typename PureFnEq<method_pointer_sig>::Args outter_args_list;
            typedef typename outter_args_list::template PopFront<>::type inner_args_list;

            std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
            std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

            std::cout << "param_list:" << param_list << std::endl;
            int indexForName = findSpace(func_sig);
            if (indexForName == -1)
            {
                return;
            }

            std::string resultType = container::repr::type_name<typename PureFnEq<method_pointer_sig>::Result>();
            std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            cpp_file << "include \"cffi.h\""
                     << std::endl
                     << resultType << " " << trait_name << "(" << header << ")"
                     << "{"
                     << std::endl
                     << "as<"
                     << trait_name
                     << ">(arg0).fn("
                     << arg_list
                     << ");"
                     << std::endl
                     << "}"
                     << std::endl;
            addFunctionBodyRecurse<typename T::MapType::TailType::KeySet>(cpp_file);
        }
    }

    //.h
    // include.include
    // all the .h from the client provided

    //.cpp
    //.h of the fi;e
    /*

    // do these have to  be by reference?
    template <typename T>
    void addParamToString(std::string &param_list)
    {
        return;
    }

    template <typename T, typename... Ts>
    void addParamToString(std::string &param_list, T const &head, Ts const &...tail)
    {
        // should we have like param names like x1, x2, x3 cuz rn its js a list of types
        // should i call type name on head -> ex container::repr::type_name<CurrTrait>()
        param_list += head + ", ";
        addParamToString(param_list, tail...);
    }
    */
    template <typename T>
    struct ParamListToString;

    // template <typename... ARGS>
    template <typename... TAIL>
    struct ParamListToString<container::TypeArray<TAIL...>>
    {
        static std::string makeString(int index, bool includeType)
        {
            std::cout << "i am going into the base case" << std::endl;
            return "";
        }
    };

    template <typename HEAD, typename... TAIL>
    struct ParamListToString<container::TypeArray<HEAD, TAIL...>>
    {
        static std::string makeString(int index, bool includeType)
        {
            std::cout << "i am going into the recursive case" << std::endl;
            std::string str_head_type = container::repr::type_name<HEAD>();
            std::string str_types_from_tail = ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType);

            std::string total_param_list = "";
            if (includeType)
            {
                total_param_list += container::repr::type_name<HEAD>();
            }
            if (container::TypeArray<TAIL...>::MapType::ITEM_COUNT == 0)
            {

                total_param_list += "arg" + std::to_string(index);
            }
            else
            {
                total_param_list += "arg" + std::to_string(index) + ", " + str_types_from_tail;
            }
            std::cout << "toal param inside tostring recursive: " << total_param_list << std::endl;
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

    // idk if we need his as its a template , i feel like i need to wire smthn so that we js filter for FFI

    void addFunctionHeaders(std::fstream &header_file)
    {
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFISet; // get every FFI specialization
        addFunctionHeaderRecurse<FFISet>(header_file);
    }

    void addFunctionBody(std::fstream &cpp_file)
    {
        std::cout << "adding function body" << std::endl;
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        addFunctionBodyRecurse<FFICppSet>(cpp_file);
    }

    /*
    void makeBody()
    {
    }


    extern "C" float b(double x)
    {
        return via<TraitB>(this).fn(x);
    }
    extern "C" float a(int x, int y)
    {
        return via<TraitA>(this).fn(x, y);
    }
        */
};

using FFIGenModule = context::SimpleModule<
    Meta<FFIGenImpl>,
    context::RequirementSet<>,
    context::ImplementationSet<FFIGen>>;

using RootModule = context::ModuleBundle<
    AModule,
    BModule,
    FFIGenModule>;

template <typename CTX>
void run()
{

    // as FFI find the FFI specializations and put into headers
    if constexpr (CTX::Info::SATISFIED)
    {

        CTX ctx{}; // constructor for components if necessary
        as<FFIGen>(ctx).genffi();
    }
}

// are we having duplicate keys bc of traitA and trait B
int main()
{
    typedef typename context::CreateContextType<
        RootModule,
        container::TypeSet<
            TraitA,
            TraitB,
            FFIEntry<TraitA>,
            FFIEntry<TraitB>,
            FFIGen>,
        Meta<
            context::EagerSolve>>::type Ctx;

    run<Ctx>();
    return 0;
}
