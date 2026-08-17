
    void addConstructor(std::fstream &gen_file, std::fstream &python_file, bool isCpp, bool is_for_CPU)
    {

        if (extern_linker_headers.size() == 0 || extern_linker_headers[0] != "extern_construct")
        {
            if (is_for_CPU){
                python_file << "extern_construct = numba.types.ExternalFunction(\n\t\"construct\",\n\tnumba.core.typing.signature(numba.types.voidptr)\n)\n\n";
                extern_linker_headers.push_back("extern_construct");               
            }
            else{
                 python_file << "extern_construct_gpu = cuda.declare_device(\n\t\""
                             << "construct_gpu\", \n\tnumba.core.typing.signature("
                             << "numba.types.voidptr"
                             << "()))\n\n";
                 extern_linker_headers.push_back("extern_construct_gpu");
            }
        }
 

        // container::repr::type_name<CONTEXT>()
        if (isCpp)
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void* construct()";
                gen_file << "{"
                         << std::endl 
                         << "\tvoid* myPtr = (void*) new "
                         << get_type_name<CONTEXT>()
                         << ";"
                         << std::endl
                         << "\tstd::cout << \"Constructor Ptr:\" << myPtr << std::endl;"
                         << std::endl
                         << "return myPtr;"
                         << "\n}"
                         << std::endl;
            }
            else{
                gen_file << "extern \"C\" int construct_gpu(void* retptr)";
                gen_file << "{"
                         << std::endl
                         << "\tretptr = (void*) new "
                         << get_type_name<CONTEXT>()
                         << ";"
                         << std::endl
                         << "\treturn 1;"
                         << "\n}"
                         << std::endl;
            }
        }
        else
        {
            if (is_for_CPU){
                gen_file << "extern \"C\" void* construct();" << std::endl;
            }
            else{
                gen_file << "extern \"C\" int construct_gpu(void* retptr);" << std::endl;
            }
        }
    }
     
