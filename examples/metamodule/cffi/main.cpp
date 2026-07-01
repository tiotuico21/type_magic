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

template <typename T>
struct PureFnEq;

template <typename RES, typename CLASS, typename... ARGS>
struct PureFnEq<RES (CLASS::*)(ARGS...)>
{
    typedef RES type(CLASS *, ARGS...);
};

template <typename CONTEXT>
struct FFIGenImpl
{
    void genffi()
    {
        std::fstream header_file;
        header_file.open("cffi.h", std::ios::trunc | std::ios::out);
        addFunctionHeaders(header_file);
        header_file.close();
    }

    template <typename T>
    void addFunctionHeaderRecurse(std::fstream &header_file)
    {
        if constexpr (std::is_same<T, container::TypeSet<>>::value)
        {
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
            // should i put it into a char pointer then
            header_file << header << std::endl;
            addFunctionHeaderRecurse<T::MapType::TailType::KeySet>(header_file);
        }
    }

    int findSpace(std::string func_sig)
    {
        for (int i = func_sig.length(); i >= 0; i--)
        {
            bool entered = false;
            int depth = 0;
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
                    return depth - 1;
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
