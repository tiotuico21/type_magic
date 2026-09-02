
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "logic.h"


using RootModule = context::ModuleBundle<AddOneModule, AddItModule, SubArgsModule, IsTrueModule, EvenModule, OddModule, StoppingTimeModule, LibRootModule>;

template <typename CTX>
void run()
{
	// std::cout << "Entered run" << std::endl;

	if constexpr (CTX::Info::SATISFIED)
	{
		//std::cout << "Context satisfied" << std::endl;

		CTX ctx{};

		//std::cout << "Context created" << std::endl;

		as<FFIGen>(ctx).genffi("logic.h", true);

		std::cout << "FFI generated" << std::endl;
	}
	else
	{
		CTX ctx{};
        std::cout << "Context not satisfied" << std::endl;
		std::cout << as<context::ContextInfo>(ctx).error_string();
	}
}
    
int main()
{
    using namespace container;
    using namespace context;

    typedef typename RootModule::template ImplFor<PublicTrait>::type PublicTraitImplMap;
    typedef typename PublicTraitImplMap::KeySet PublicTraitImplSet;

    typedef TypeMap<Binding<key::RootModule, RootModule>> BaseInputState;

typedef typename BaseInputState
    ::template SetItem<key::RequirementSet, 
TypeSet<AddOne, AddIt, SubArgs, IsTrue, Even, Odd, StoppingTime, FFIEntry<AddOne>, FFIEntry<AddIt>, FFIEntry<SubArgs>, FFIEntry<IsTrue>, FFIEntry<Even>, FFIEntry<Odd>, FFIEntry<StoppingTime>, SubOne, FFIEntry<SubOne>, Print<int>, Print<float>, FFIEntry<Print<int>>, FFIEntry<Print<float>>, Log<int>, Log<float>, FFIEntry<Log<int>>, FFIEntry<Log<float>>, FFIGen>
			>::type StandardTraits;
        run<typename context::CreateContextType<StandardTraits>::type>();
        return 0;
    }
    