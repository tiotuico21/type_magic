#include <thread>
#include "../../../include/include.h"
#include <functional>
#include <iostream>

struct TraitA
{
};

struct TraitB
{
};

struct FFI
{
};

struct B
{
    float fn(double x)
    {
        float returnVal = x * 3.0;
        return returnVal;
    }
};

using BModule = context::SimpleModule<
    Meta<B>,
    context::RequirementSet<TraitA>,
    context::ImplementationSet<TraitB, FFI<TraitB>>>;

struct A
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
    Meta<A>,
    context::RequirementSet<TraitB>,
    context::ImplementationSet<TraitA, FFI<TraitA>>>;

template <typename CONTEXT>
struct FFIGen
{
    void genffi()
    {
        makeHeaders();
        makeBody();
    }

    void makeHeaders()
    {
    }

    void makeBody()
    {
    }

    extern "C" float b(double x)
    {
        return via<TraitA>(this).fn(x);
    }
    extern "C" float a(int x, int y)
    {
        return via<TraitB>(this).fn(x, y);
    }
};
