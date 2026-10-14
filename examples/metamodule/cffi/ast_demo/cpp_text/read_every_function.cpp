    // Helper: add indentation to every line AFTER the first line.
    // The first line stays where it was inserted.
    std::string indent_after_first_line(
        const std::string& str,
        int spaces
    )
    {
        std::string result = str;
        std::string indent(spaces, ' ');

        for (size_t i = 0; i < result.size(); ++i) {
            if (result[i] == '\n' && i + 1 < result.size()) {
                result.insert(i + 1, indent);

                // Skip over the indentation we just inserted
                i += spaces;
            }
        }

        return result;
    }


    static void write_extern_to_python_file_cpu(std::fstream &python_file,
                                    std::string typemagic_mangle_name,
                                    std::string extern_function_return_type,
                                    std::string extern_function_param_list){
        //std::cout << "Not print: " << get_type_name<KEY>().substr(0, 5)  << std::endl;
                    
        std::string extern_python_function = "extern_" + typemagic_mangle_name + " = nb.types.ExternalFunction(\n\t\""
                                + typemagic_mangle_name
                                + "\",\n\tnb.core.typing.signature(\n\t\t"
                                + extern_function_return_type + ", \n\t\t"
                                + extern_function_param_list
                                + "\n\t)\n)\n\n";

        python_file << extern_python_function;
        //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name));
        extern_linker_headers.push_back("extern_" + typemagic_mangle_name);

    }


    static void write_extern_to_python_file_gpu(std::fstream &python_file,
                                                std::string reg_str_func_name,
                                                std::string typemagic_mangle_name,
                                                std::string extern_function_return_type,
                                                std::string extern_function_param_list){
         python_file << "extern_" << toSnakeCase(reg_str_func_name) << "_gpu = cuda.declare_device(\n\t\""
                    << typemagic_mangle_name + "_gpu" << "\", \n\tnb.core.typing.signature("
                    << extern_function_return_type
                    << "(" << extern_function_param_list << ")))\n\n";
        //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name) + "_gpu");
        extern_linker_headers.push_back("extern_" + typemagic_mangle_name);
    }
    static void write_function_to_cpp_file_cpu(std::fstream &gen_file,
                                  std::string resultType,
                                  std::string typemagic_mangle_name,
                                  std::string header,
                                  std::string traitName,
                                  std::string reg_str_func_name,
                                  std::string arg_list){
        gen_file << "extern \"C\" "
                << resultType << " " << typemagic_mangle_name << "(" << header << ")"
                << "{"
                << std::endl
                << "\t"
                << get_type_name<CONTEXT>()
                << "* ptr = ("
                << get_type_name<CONTEXT>()
                << "*) arg0;"
                << std::endl
                << "\treturn As<"
                << traitName
                << ", "
                << get_type_name<CONTEXT>()
                << ">::STable::template call<typename "
                << reg_str_func_name
                << ">(&(as<"
                << traitName;
        if (arg_list.size() == 0)
        {
            gen_file << ">(*ptr)));";
        }
        else
        {
            gen_file << ">(*ptr)), "
                        << arg_list
                        << ");";
        }
        gen_file << std::endl
                    << "}"
                    << std::endl;
    
    }


    static void write_function_to_cpp_file_gpu(std::fstream &gen_file,
                                               std::string typemagic_mangle_name,
                                               std::string resultType,
                                               std::string header,
                                               std::string traitName,
                                               std::string reg_str_func_name,
                                               std::string arg_list){
        gen_file << "extern \"C\" "
                << "int" << " " << typemagic_mangle_name + "_gpu" << "(" << resultType + "* retptr, " + header << ")"
                << "{"
                << std::endl
                << "\t"
                << get_type_name<CONTEXT>()
                << "* ptr = ("
                << get_type_name<CONTEXT>()
                << "*) arg0;"
                << std::endl
                << "\t*retptr = As<"
                << traitName
                << ", "
                << get_type_name<CONTEXT>()
                << ">::STable::template call<typename "
                << reg_str_func_name
                << ">(&(as<"
                << traitName;
        if (arg_list.size() == 0)
        {
            gen_file << ">(*ptr)));";
        }
        else
        {
            gen_file << ">(*ptr)), "
                        << arg_list
                        << ");";
        }
        gen_file << std::endl
                    << "\treturn 1;"
                    << "\n}"
                    << std::endl;
    }
   
    static void write_header_to_cpp_file_cpu(std::fstream &gen_file,
                                             std::string resultType,
                                             std::string typemagic_mangle_name,
                                             std::string header)
    {
        gen_file << "extern \"C\" "
            << resultType
            << " "
            << typemagic_mangle_name
            << "("
            << header
            << ")"
            << ";"
            << std::endl;
    }

    static void write_header_to_cpp_file_gpu(std::fstream &gen_file,
                                             std::string resultType,
                                             std::string typemagic_mangle_name,
                                             std::string header)
    {
        gen_file << "extern \"C\" "
            << "int"
            << " "
            << typemagic_mangle_name + "_gpu"
            << "("
            << resultType + "* retptr, " + header
            << ")"
            << ";"
            << std::endl;
    }

    static std::string make_njit_param_list(int param_amount){
        std::string param_list = "ptr";
        if (param_amount == 0){
            return "ptr";
        }
        for (int i = 1; i <= param_amount; i++){
            std::string arg = "arg" + std::to_string(i);
            param_list += ", " + arg;
        }
        return param_list;
    }
   
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
            std::string mangle_func_name = typeid(KEY).name();
            std::string typemagic_mangle_name = "_TYPEMAGIC" + mangle_func_name;
            size_t pos = typemagic_mangle_name.find("N");

            if (pos != std::string::npos) {
                typemagic_mangle_name.erase(pos + 1, 1);
            }
            std::string reg_str_func_name = get_type_name<KEY>();

            std::cout << "________________________REGULAR FUNC" << reg_str_func_name << std::endl;

            if (isCpp)
            {
                typedef typename ITEM::Args method_args_list;

                std::cout << "\n\nITEMMMMMMM COUNT" << std::endl;
                std::cout << method_args_list::MapType::ITEM_COUNT << std::endl;
                int param_amount = method_args_list::MapType::ITEM_COUNT;

                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;
                typedef typename outter_args_list::template PopFront<CONTEXT *>::type inner_args_list;
                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);
                std::string arg_list = ParamListToString<inner_args_list>::makeString(1, false);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

            
                std::string extern_function_return_type = "nb." + cppToNumbaType(resultType);
                std::string extern_function_param_list = ParamListToString<outter_args_list>::makeString(0, true, true);


                if (is_for_CPU){
                    //python_file << "extern_" << toSnakeCase(reg_str_func_name) << " = numba.types.ExternalFunction(\n\t\""
                    
                  
                        /*
                        @nb.njit(cache=False)
                    def add_one(ptr, arg1, arg2, arg3):
                        return extern__TYPEMAGICNAddOne6CallFnE(ptr, arg1, arg2, arg3)

                        extern_linker_headers.push_back("extern_" + typemagic_mangle_name);

                          for (size_t i = 0; i < extern_func_headers.size(); ++i)
        {
            python_file << "@nb.njit(cache=False)\n";
            python_file << extern_func_headers[i] << "\n\t"
                        << "return " << extern_linker_headers[i] << extern_func_param[i] << "\n\n";
        }
                        */
                
                    write_extern_to_python_file_cpu(python_file,
                                                typemagic_mangle_name, 
                                                extern_function_return_type,
                                                extern_function_param_list);

                    write_function_to_cpp_file_cpu(gen_file,
                                               resultType,
                                               typemagic_mangle_name,
                                               header,
                                               traitName, 
                                               reg_str_func_name,
                                               arg_list);

                    python_file << "@nb.njit(cache=False)\n";
                    std::string njit_param_list = make_njit_param_list(param_amount);
                    python_file << "def " + toSnakeCase(traitName) + "(" + njit_param_list + "):\n\t"
                                << "return extern_" << typemagic_mangle_name
                                << "(" << njit_param_list + ")\n\n";

                


                    /*
                    @nb.njit(cache=False)
                    def add_one(ptr, arg1, arg2, arg3):
                        return extern__TYPEMAGICNAddOne6CallFnE(ptr, arg1, arg2, arg3)

                    */

                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{

                    write_extern_to_python_file_gpu(python_file,
                                                    reg_str_func_name,
                                                    typemagic_mangle_name, 
                                                    extern_function_return_type,
                                                    extern_function_param_list);
                    //extern_linker_headers.push_back("extern_" + toSnakeCase(reg_str_func_name) + "_gpu");
                   
                    write_function_to_cpp_file_gpu(gen_file,
                                                   typemagic_mangle_name,
                                                   resultType,
                                                   header,
                                                   traitName, 
                                                   reg_str_func_name,
                                                   arg_list);
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(true, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
            }
            else
            {
                typedef typename ITEM::Args method_args_list;
                typedef typename method_args_list::template PushFront<CONTEXT *>::type outter_args_list;

                std::string param_list = ParamListToString<outter_args_list>::makeString(0, true);

                std::string resultType = get_type_name<typename ITEM::Result>();
                std::string header = param_list; // func_sig.substr(0, indexForName) + " " + trait_name + func_sig.substr(indexForName + 1);

                if (is_for_CPU){
                    write_header_to_cpp_file_cpu(gen_file,
                                                 resultType,
                                                 typemagic_mangle_name,
                                                 header);
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                else{
                    write_header_to_cpp_file_cpu(gen_file,
                                                 resultType,
                                                 typemagic_mangle_name,
                                                 header);
                    ReadEveryFunction<container::TypeMap<TAIL...>>::exec(false, traitName, gen_file, python_file, is_for_CPU, func_index + 1);
                }
                
                // ParamListToString<container::TypeArray<TAIL...>>::makeString(index + 1, includeType)
            }
        }
    };


    
