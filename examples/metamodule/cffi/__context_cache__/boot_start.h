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


