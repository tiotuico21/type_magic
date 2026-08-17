
    void addDestructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if (extern_linker_headers.size() <= 1 || extern_linker_headers[1] != "extern_destruct")
        {
            if (is_for_CPU){
                python_file << "extern_destruct = numba.types.ExternalFunction(\n\t\"destructor\",\n\tnumba.core.typing.signature(\n\t\tnumba.types.voidptr, numba.types.voidptr\n\t)\n)\n\n";
                extern_linker_headers.push_back("extern_destruct");
            }
            else{
                python_file << "extern_destruct_gpu = cuda.declare_device(\n\t\""
                            << "destructor_gpu\", \n\tnumba.core.typing.signature("
                            << "numba.types.void"
                            << "(numba.types.voidptr)))\n\n";
                extern_linker_headers.push_back("extern_destruct_gpu");
            }
        }

        if (isCpp)
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void destructor(void* ptr)";
                gen_file << "{"
                         << std::endl
                         << get_type_name<CONTEXT>()
                         << "*ptr_to_delete = ("
                         << get_type_name<CONTEXT>()
                         << "*) ptr;"
                         << "\n\treturn delete ptr_to_delete;"
                                     
                         << "\n}\n\n";
            }
            else{
                //gen_file << "extern \"C\" __device__\n";
                gen_file << "extern \"C\" int destructor_gpu(void* retptr, void* ptr)";
                gen_file << "{"
                         << std::endl
                         << get_type_name<CONTEXT>()
                         << "*ptr_to_delete = ("
                         << get_type_name<CONTEXT>()
                         << "*) ptr;"
                         << "\n\tdelete ptr_to_delete;"
                         << "\n\treturn 1;"
                         << "\n}\n";
            }
        }
        else
        {
            if (is_for_CPU){
               gen_file << "extern \"C\" void destructor(void* ptr);" << std::endl;
            }
            else{
               gen_file << "extern \"C\" int destructor_gpu(void* retptr, void* ptr);" << std::endl;
            } 
        } 
    }

