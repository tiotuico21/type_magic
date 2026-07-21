#include <iostream>
#include <fstream>
#include <unistd.h>
#include "logic.h"

using RootModule = context::ModuleBundle<AddOne, AddIt>;

template <typename CTX>
void run()
{
	if constexpr (CTX::Info::SATISFIED){
		CTX ctx{};
		as<FFIGen>(ctx).genffi("logic.h");
	}
}

int main()
{
	typedef typename context::CreateContextType<
		RootModule,
		container::TypeSet<AddOne, AddIt, FFIEntry<AddOne>, FFIEntry<AddIt>, FFIGen>,
		Meta<context::EagerSolve>>::type Ctx;

	run<Ctx>();
	return 0;
}
