
    void addDestructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if ((extern_linker_headers.size() <= 1 || extern_linker_headers[1] != "extern_destruct") && isCpp)
        {
            if (is_for_CPU){
                python_file << "extern_destruct = nb.types.ExternalFunction(\n\t\"destructor\",\n\tnb.core.typing.signature(\n\t\tnb.types.voidptr, nb.types.voidptr\n\t)\n)\n\n";
                python_file << "@nb.njit(cache=False)\n"
                        << "def destruct(ptr):\n\t"
                        << "return destruct(ptr)\n\n";          
            }
            else{
                python_file << "extern_destruct_gpu = cuda.declare_device(\n\t\""
                            << "destructor_gpu\", \n\tnb.core.typing.signature("
                            << "nb.types.void"
                            << "(nb.types.voidptr)))\n\n";
                python_file << "@nb.njit(cache=False)\n"
                    << "def destruct(ptr):\n\t"
                    << "return destruct(ptr)\n\n";
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

