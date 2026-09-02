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


