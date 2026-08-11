
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
        typedef typename CONTEXT::TraitMap::KeySet::template Filter<Meta<FFIEntry>::template Generalizes>::type FFICppSet; // get every FFI specialization
        addFunctionGenRecurse<FFICppSet>(cpp_file, python_file, true, is_for_CPU);
        addDestructor(cpp_file, python_file, true, is_for_CPU);
    }

