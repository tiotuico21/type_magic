#include "root.h"

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
struct Even{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int(void*, int)>>
	STable;
};
struct Odd{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int(void*, int)>>
	STable;
};
struct StoppingTime{
	struct CallFn{};
	typedef StaticTable<
		container::Binding<CallFn, int(void*, int)>>
	STable;
};

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
	context::RequirementSet<SubOne>,
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


extern "C" int even(void* ctx, int arg1);
template <typename CONTEXT>
struct ImplEven{
	int call(int arg1){
		int result;
		return even((CONTEXT*)this,arg1);
	}
	
typedef StaticTable<
        container::Binding<
Even::CallFn, Fn<&ImplEven<CONTEXT>::call>>>
		STable;

};
using EvenModule = context::SimpleModule<
	Meta<ImplEven>,
	context::RequirementSet<>,
	context::ImplementationSet<Even, FFIEntry<Even>>
>;


extern "C" int odd(void* ctx, int arg1);
template <typename CONTEXT>
struct ImplOdd{
	int call(int arg1){
		int result;
		return odd((CONTEXT*)this,arg1);
	}
	
typedef StaticTable<
        container::Binding<
Odd::CallFn, Fn<&ImplOdd<CONTEXT>::call>>>
		STable;

};
using OddModule = context::SimpleModule<
	Meta<ImplOdd>,
	context::RequirementSet<>,
	context::ImplementationSet<Odd, FFIEntry<Odd>>
>;


extern "C" int stopping_time(void* ctx, int arg1);
template <typename CONTEXT>
struct ImplStoppingTime{
	int call(int arg1){
		int result;
		return stopping_time((CONTEXT*)this,arg1);
	}
	
typedef StaticTable<
        container::Binding<
StoppingTime::CallFn, Fn<&ImplStoppingTime<CONTEXT>::call>>>
		STable;

};
using StoppingTimeModule = context::SimpleModule<
	Meta<ImplStoppingTime>,
	context::RequirementSet<>,
	context::ImplementationSet<StoppingTime, FFIEntry<StoppingTime>>
>;



