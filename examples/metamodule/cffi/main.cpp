
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "logic.h"

using RootModule = context::ModuleBundle<
    AModule,
    BModule,
    AllocModule,
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
    /*
    typedef typename context::CreateContextType<
        RootModule,
        container::TypeSet<
            TraitA,
            TraitB,
            Alloc,
            FFIEntry<Alloc>,
            FFIEntry<TraitA>,
            FFIEntry<TraitB>,
            FFIGen>,
        Meta<
            context::EagerSolve>>::type Ctx;
    */
    using namespace container;   
    using namespace context;   
    typedef TypeMap<Binding<key::RootModule, RootModule>> BaseInputState;

    typedef typename BaseInputState
        ::template SetItem<key::RequirementSet, TypeSet<
            TraitA,
            TraitB,
            Alloc,
            FFIEntry<Alloc>,
            FFIEntry<TraitA>,
            FFIEntry<TraitB>,
            FFIGen
        >
    >::type StandardTraits;
    run<typename context::CreateContextType<StandardTraits>::type>();
    
   // run<Ctx>();
    return 0;
}

/*
 using namespace container;   
    using namespace context;   

    typedef TypeMap<Binding<key::RootModule,RootModule>> BaseInputState;

    typedef typename BaseInputState
            ::template SetItem<key::RequirementSet,TypeSet<FileLog,StandardLogStyle>>::type
            StandardFile;
*/