
    void genffi(std::string client_logic_header_file, bool is_for_CPU)
    {
        std::fstream header_file;
        std::fstream cpp_file;
        std::fstream python_file;
        header_file.open("cffi.h", std::ios::trunc | std::ios::out);
        cpp_file.open("cffi.cpp", std::ios::trunc | std::ios::out);
        python_file.open("test_ffi.py", std::ios::trunc | std::ios::out);

        python_file << "import re\n"
                    << "import sys\n"
                    << "import numba\n"
                    << "import inspect\n"
                    << "import subprocess\n"
                    << "\n"
                    << "from llvmlite import binding\n"
                    << "\n"
                    << "binding.load_library_permanently(\"./my_dynamic_library.so\")\n";

        addFunctionHeaders(header_file, python_file, client_logic_header_file, is_for_CPU);
        addFunctionBody(cpp_file, python_file, is_for_CPU);
        header_file.close();
        cpp_file.close();

        for (size_t i = 0; i < extern_func_headers.size(); ++i)
        {
            python_file << "@numba.njit(cache=False)\n";
            python_file << extern_func_headers[i] << "\n\t"
                        << "return " << extern_linker_headers[i] << extern_func_param[i] << "\n\n";
        }
        std::cout << python_file.is_open() << '\n';
        python_file.close();

        // why fPIC smthn about address reolacation
        

       
    }

