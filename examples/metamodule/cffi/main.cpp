#include <thread>
#include "../../../include/include.h"
#include <functional>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "logic.h"

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
        // CTX ctx(As<FFIGen, CTX>{});
        CTX ctx{}; // constructor for components if necessary
        as<FFIGen>(ctx).genffi("logic.h");
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
