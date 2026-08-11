
template <typename T>
struct PrintImplMeta{
    template <typename CONTEXT>
    struct PrintImpl{
        void my_print(T item){
            std::cout << "Print: " << item << std::endl;
        }
        typedef StaticTable<
            container::Binding<
                typename Print<T>::PrintFn,
                Fn<&PrintImplMeta::template PrintImpl<CONTEXT>::my_print>
            >
        >
        STable;
    };

    typedef context::SimpleModule <
        Meta<PrintImpl>,
        context::RequirementSet<>,
        context::ImplementationSet<Print<T>, FFIEntry<Print<T>>>
    > Module;
};

template <typename T>
struct PrintImplFFIMeta {
    typedef context::EmptyModule Module;
};

template <typename T>
struct PrintImplFFIMeta <Print<T>> { 
    typedef context::SimpleModule <
            Meta<PrintImplMeta<T>::template PrintImpl>,
            context::RequirementSet<>,
            context::ImplementationSet<Print<T>, FFIEntry<Print<T>>>
        > Module;
};

