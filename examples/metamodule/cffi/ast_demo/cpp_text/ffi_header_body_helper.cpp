
    template <typename T>
    struct IsSpecialization{
        static constexpr bool value = false;
    };
    
    template <template<typename...>typename TEMPLATE, typename T>
    struct IsSpecialization<TEMPLATE<T>>{
        static constexpr bool value = true;
    };
    template <typename T>
    struct GetTemplate{
        typedef T type;
    };

    template <template <typename...>typename TEMPLATE, typename T>
    struct GetTemplate<TEMPLATE<T>>{
        typedef Meta<TEMPLATE> type;
    };

    template <typename TRAITMAP>
    struct FFIDetector {
        template <typename TRAIT>
        struct HasFFI{
            static constexpr bool value = TRAITMAP::template has_key<TRAIT>() && TRAITMAP::template has_key<FFIEntry<TRAIT>>();
        };
    };


    /*
    template <typename T>
void put_in_dict_file(std::fstream& dict_file){
    using namespace container;
    using namespace context;

    if constexpr (std::is_same<T, container::TypeSet<>>::value)
    {
        return;
    }
    else{
        typedef typename T::MapType::HeadItemType CurrTrait;

        if (IsMeta<CurrTrait>::value){
            std::string FullTrait = container::repr::type_name<CurrTrait>();
            size_t start = FullTrait.find('<');
            size_t end = FullTrait.find('>', start); 

            std::string InnerTrait = FullTrait.substr(start + 1, end - start -1);
            
            dict_file << "def " << InnerTrait << "(*args):";
            dict_file << "\n\treturn {cpp_name: \"" << InnerTrait
                      << "<\"+\",\".join(*args)+\">\"}\n";
            put_in_dict_file<typename T::MapType::TailType::KeySet>(dict_file);
        }
        else{
            typedef typename T::MapType::HeadItemType CurrTrait;
            dict_file << container::repr::type_name<CurrTrait>();
            dict_file << " = { cpp_name: \"" << container::repr::type_name<CurrTrait>() << "\"}\n\n";
            put_in_dict_file<typename T::MapType::TailType::KeySet>(dict_file);
        }
         
    }
}
    */
    //CFFIMeta::KeySet::template FIler<M::template Generalizes>::type my_meta_set


    /*
      std::cout << "*************KEY: " << reg_str_func_name.substr(0, 5)  << std::endl;
                        size_t start = reg_str_func_name.find('<');
                        size_t end = reg_str_func_name.find('>', start);

                        std::string cpp_print_type = reg_str_func_name.substr(start + 1, end - start - 1);
                        
                        std::string numba_type = "nb." + cppToNumbaType(cpp_print_type);
                        meta_specialization.push_back(numba_type);
                        std::string extern_python_function =
                        "nb.types.ExternalFunction(\n"
                        "    \"" + typemagic_mangle_name + "\",\n"
                        "    nb.core.typing.signature(\n"
                        "        " + extern_function_return_type + ",\n"
                        "        " + extern_function_param_list + "\n"
                        "    )\n"
                        ")";

                      
                        extern_meta_headers.push_back(extern_python_function);
    */

    /*
    
    template <typename... T>
    struct ReadEveryFunction;

    template <typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<TAIL...>>
    {

        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {
            return;
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadEveryFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
        static void exec(bool isCpp, std::string traitName, std::fstream &gen_file, std::fstream &python_file, bool is_for_CPU, int func_index = 0)
        {

         typedef typename T::MapType::HeadItemType CurrFFISpec;
            typedef typename GetTemplateArgs<CurrFFISpec>::template ItemAt<0>::type CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::string trait_name = get_type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCase(trait_name);

            typedef As<CurrTrait, CONTEXT> sig_component;

            std::string func_sig = get_type_name<CONTEXT>(); 
            if (isCpp)
            {
                ReadEveryFunction<typename sig_component::STable::EntriesTypeMap>::exec(true, trait_n
    */
    template <typename T>
    void handle_every_generic_trait(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU){
        if constexpr(std::is_same<T, container::TypeSet<>>::value){
            return;
        }
        else{
            typedef typename T::MapType::HeadItemType CurrTrait; 
            std::cout << "\n\nHEad: " << container::repr::type_name<CurrTrait>() << "/n/n" << std::endl;
         

            typedef typename  CONTEXT::TraitMap::KeySet::template Filter<CurrTrait::template Generalizes>::type MyMetaSet;
            std::string meta_trait_name = get_type_name<CurrTrait>();
            std::string meta_camel_name = toSnakeCaseMeta(meta_trait_name);
            std::cout << meta_camel_name << std::endl;
            std::cout << "METAAAAAAAAA SET:" << container::repr::type_name<MyMetaSet>() << "/n/n" << std::endl;
            
            make_extern_map_for_single_meta_trait<MyMetaSet>(gen_file, python_file, meta_camel_name, isCpp, is_for_CPU);
            handle_every_generic_trait<typename T::MapType::TailType::KeySet>(gen_file, python_file, isCpp, is_for_CPU);

        }
    }

    //traverse the specializations of Meta<print>
    template <typename CURRSET>
    void make_extern_map_for_single_meta_trait(std::fstream &gen_file, std::fstream &python_file, std::string meta_name, bool isCpp, bool is_for_CPU){
        std::string extern_map = meta_name + "_ext_map = {";
        std::string all_specialization_entries = make_all_meta_map_entries<CURRSET>(gen_file, python_file, meta_name, isCpp, is_for_CPU);
        std::cout << "\n\nSpecializerssss: " << all_specialization_entries << std::endl;


        std::string complete_extern_map_for_single_trait =
            meta_name + "_ext_map = {" +
            indent(all_specialization_entries) +
            "\n}\n";
        
        std::cout << complete_extern_map_for_single_trait << std::endl;
        python_file << complete_extern_map_for_single_trait;
    }

    template <typename CURRSET>
    static std::string make_all_meta_map_entries(std::fstream &gen_file, std::fstream &python_file, std::string meta_name, bool isCpp, bool is_for_CPU){
        if constexpr(std::is_same<CURRSET, container::TypeSet<>>::value){
            return "";
        }
        else{
            std::string result = "";
            typedef typename CURRSET::MapType::HeadItemType CurrTrait;
            std::string typenameMangle = typeid(CurrTrait).name();
            std::string generated_func_name = "_TYPEMAGIC" + typenameMangle + container::repr::type_name<CurrTrait>();
            std::string trait_name = get_type_name<CurrTrait>();
            std::string trait_name_snake_case = toSnakeCaseMeta(trait_name);

            std::cout << "\n\nTRAITSSSS " << trait_name_snake_case << std::endl;

            typedef As<CurrTrait, CONTEXT> sig_component;
            std::string single_entry = ReadMetaFunction<typename sig_component::STable::EntriesTypeMap>::exec(isCpp, trait_name, gen_file, python_file, is_for_CPU, "");
            std::cout << "single_entry: " << single_entry << std::endl;
            result += single_entry;
            result += make_all_meta_map_entries<typename CURRSET::MapType::TailType::KeySet>(gen_file, python_file, meta_name, isCpp, is_for_CPU);
            return result;
        }
    }

    static std::string meta_single_entry_generator(std::string typemagic_mangle_name,
                                            std::string extern_function_return_type,
                                            std::string extern_function_param_list,
                                            std::string reg_str_func_name,
                                            std::string traitName){
        std::string map_entry = "";
        size_t start = reg_str_func_name.find('<');
        size_t end = reg_str_func_name.find('>', start);

        std::string cpp_print_type = reg_str_func_name.substr(start + 1, end - start - 1);
        
        std::string numba_type = "nb." + cppToNumbaType(cpp_print_type);
        //meta_specialization.push_back(numba_type);
        std::string meta_handle = "my_" + traitName + "_type(" + numba_type + "):";
        map_entry = std::format(R"PY(
my_print_type({}): nb.types.ExternalFunction(
    "{}",
    nb.core.typing.signature(
        {},
        {}
    )
),)PY",
        numba_type,
        typemagic_mangle_name,
        extern_function_return_type,
        extern_function_param_list
        );
        return map_entry;
    }

      

    template <typename... T>
    struct ReadMetaFunction;

    template <typename... TAIL>
    struct ReadMetaFunction<container::TypeMap<TAIL...>>
    {
        static std::string exec(bool isCpp, 
                           std::string traitName, 
                           std::fstream &gen_file, 
                           std::fstream &python_file, 
                           bool is_for_CPU, 
                           std::string result,
                           int func_index = 0)
        {
            return "";
        }
    };

    template <typename KEY, typename ITEM, typename... TAIL>
    struct ReadMetaFunction<container::TypeMap<container::Binding<KEY, ITEM>, TAIL...>>
    {
         static std::string exec(bool isCpp, 
                           std::string traitName, 
                           std::fstream &gen_file, 
                           std::fstream &python_file, 
                           bool is_for_CPU, 
                           std::string result,
                           int func_index = 0)
        {
            std::string mangle_func_name = typeid(KEY).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string reg_str_func_name = get_type_name<KEY>();

            std::cout << "88888888888MEta FUNC" << reg_str_func_name << std::endl;

            std::string map_entry = "";
            if (isCpp){
                typedef typename ITEM::Args method_args_list;

                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            
                std::string extern_function_return_type = "nb." + cppToNumbaType(resultType);
                std::string extern_function_param_list = ParamListToString<outter_args_list>::makeString(0, true, true);


                map_entry = meta_single_entry_generator(typemagic_mangle_name,
                                            extern_function_return_type,
                                            extern_function_param_list,
                                            reg_str_func_name,
                                            traitName);
            }
            return map_entry + ReadMetaFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, result, func_index + 1);
        }
    };

    static std::string indent(
    const std::string& text,
    const std::string& prefix = "    ")
    {
        std::string result;
        result.reserve(text.size() + 64);

        bool beginning_of_line = true;

        for (char c : text)
        {
            if (beginning_of_line && c != '\n')
            {
                result += prefix;
                beginning_of_line = false;
            }

            result += c;

            if (c == '\n')
                beginning_of_line = true;
        }

        return result;
    }

    /*
        {

    /*
    template <typename CURRSET>
    std::string make_extern_map_entries(std::string meta_name){
        if constexpr(std::is_same<T, container::TypeSet<>>::value){
            return;
        }
        else{
            typedef typename T::MapType::HeadItemType CurrSpecialization; 
            std::string mangle_func_name = typeid(CurrSpecialization).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string extern_python_function =
                    "nb.types.ExternalFunction(\n"
                    "    \"" + typemagic_mangle_name + "\",\n"
                    "    nb.core.typing.signature(\n"
                    "        " + extern_function_return_type + ",\n"
                    "        " + extern_function_param_list + "\n"
                    "    )\n"
                    ")";


        }
    }
    */
        
    void addFunctionHeaders(std::fstream &header_file, std::fstream &python_file, std::string client_header, bool is_for_CPU)
    {
        header_file << "#include <thread>"
                    << std::endl
                    << "#include \"../../../../include/include.h\""
                    << std::endl
                    << "#include <functional>"
                    << std::endl
                    << "#include <iostream>"
                    << std::endl
                    << "#include <fstream>"
                    << std::endl
                    << "#include \"" << client_header << "\""
                    << std::endl;

        addConstructor(header_file, python_file, false, is_for_CPU);
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFISet; // get every FFI specialization
        addFunctionGenRecurse<FFISet>(header_file, python_file, false, is_for_CPU);
        addDestructor(header_file, python_file, false, is_for_CPU);
    }

    void addFunctionBody(std::fstream &cpp_file, std::fstream &python_file, bool is_for_CPU)
    {
        //std::cout << "adding function body" << std::endl;
        cpp_file << "#include \"cffi.h\""
                 << std::endl
                 << std::endl;
        addConstructor(cpp_file, python_file, true, is_for_CPU);
    
       //filter will put every trait into isspecialization
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<IsSpecialization>::type MySet;
        std::cout << "THIS IS A SPECIALIZE TRAIT: " << container::repr::type_name<MySet>() << std::endl;
        //typedef typename MySet::template LossyMap<GetTemplate>::type MyMetaSet
        std::cout << "THIS IS ALL MY META TRAITS: " << container::repr::type_name<MySet>() << std::endl;
        typedef typename MySet::template Filter<FFIDetector<typename CONTEXT::TraitMap>::template HasFFI>::type FFISetUnMeta;
        typedef typename FFISetUnMeta::template LossyMap<GetTemplate>::type FFIMetaSet;
        //handle_every_generic_trait<MyMetaSet>(python_file);
        std::cout << "\n\nTRAITS THAT IMPLEMENT FFI" << container::repr::type_name<FFIMetaSet>() << std::endl;
        handle_every_generic_trait<FFIMetaSet>(cpp_file, python_file, true, is_for_CPU);

        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        addFunctionGenRecurse<FFICppSet>(cpp_file, python_file, true, is_for_CPU);
        addDestructor(cpp_file, python_file, true, is_for_CPU);
    }

