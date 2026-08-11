
template <typename T>
struct Print{
    struct PrintFn{};

    typedef StaticTable<
        container::Binding<PrintFn, void(T)>>
        STable;
};

