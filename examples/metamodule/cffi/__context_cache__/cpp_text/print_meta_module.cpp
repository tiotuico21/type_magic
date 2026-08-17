
typedef context::ModuleBundle<
    context::MetaModule <
        Print,
        PrintImplMeta
    >,
    context::MetaModule <
        FFIEntry,
        PrintImplFFIMeta
    >
> PrintModule;

