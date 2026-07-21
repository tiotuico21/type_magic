#include <typeinfo>
#include "../../../include/include.h"

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


struct AddOne{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int* (int*, void*, int, std::string, bool)>>
	STable;
};
struct AddIt{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int* (int*, void*)>>
	STable;
};

extern "C" int add_one(int* ret, int arg1, std::string arg2, bool arg3);
template <typename CONTEXT>
struct ImplAddOne{
	int call(int arg1, std::string arg2, bool arg3){
		int result;
		add_one(&result, (CONTEXT*)this,arg1, arg2, arg3, arg4);
		return result;
	}
};
using AddOneModule = context::SimpleModule<
	Meta<ImplAddOne>,
	context::RequirementSet<>,
	context::ImplementationSet<AddOne, FFIEntry<AddOne>>
>;

extern "C" int add_it(int* ret);
template <typename CONTEXT>
struct ImplAddIt{
	int call(){
		int result;
		add_it(&result, (CONTEXT*)this);
		return result;
	}
};
using AddItModule = context::SimpleModule<
	Meta<ImplAddIt>,
	context::RequirementSet<>,
	context::ImplementationSet<AddIt, FFIEntry<AddIt>>
>;


template <typename T>
struct PureFnEq;

template <typename RES, typename CLASS, typename... ARGS>
struct PureFnEq<RES (CLASS::*)(ARGS...)>
{
	typedef RES type(CLASS *, ARGS...);
	typedef container::TypeArray<CLASS *, ARGS...> Args;
	typedef RES Result;
};

