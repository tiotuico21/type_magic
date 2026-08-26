#include <iostream>
#include <fstream>
#include <unistd.h>
#include "boot_start.h"

using RootModule = context::ModuleBundle<SubOneModule, FFIGenModule>;


template <typename CTX>
void run()
{
	// std::cout << "Entered run" << std::endl;

	if constexpr (CTX::Info::SATISFIED)
	{
		//std::cout << "Context satisfied" << std::endl;

		CTX ctx{};

		//std::cout << "Context created" << std::endl;

		as<FFIGen>(ctx).genffi("logic.h", true);

		std::cout << "FFI generated" << std::endl;
	}
	else
	{
		CTX ctx{};
        std::cout << "Context not satisfied" << std::endl;
		std::cout << as<context::ContextInfo>(ctx).error_string();
	}
}
static std::string cppTypeToPythonHandle(const std::string& type)
{
    std::string result;


    // Replace C++ template syntax with underscores
    for (char c : type)
    {
        if (c == '<' || c == '>' || c == ',' || c == ' ')
        {
            result += '_';
        }
        else
        {
            result += c;
        }
    }


    // Remove duplicate underscores
    std::string cleaned;
    bool last_was_underscore = false;


    for (char c : result)
    {
        if (c == '_')
        {
            if (!last_was_underscore)
            {
                cleaned += c;
            }


            last_was_underscore = true;
        }
        else
        {
            cleaned += c;
            last_was_underscore = false;
        }
    }


    // Remove trailing underscore
    if (!cleaned.empty() && cleaned.back() == '_')
    {
        cleaned.pop_back();
    }


    return cleaned;
}
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
        std::string full_trait = container::repr::type_name<CurrTrait>();

        std::string python_handle = cppTypeToPythonHandle(full_trait);
        if (IsMeta<CurrTrait>::value){
            size_t start = full_trait.find('<');
            size_t end = full_trait.find('>', start); 

            std::string inner_trait = full_trait.substr(start + 1, end - start -1);
            
            dict_file << "def " << inner_trait << "(*args):";
            dict_file << "\n\treturn {cpp_name: \"" << inner_trait
                      << "<\"+\",\".join(*args)+\">\"}\n";
            put_in_dict_file<typename T::MapType::TailType::KeySet>(dict_file);
        }
        else{
            dict_file << python_handle;
            dict_file << " = { \"cpp_name\": \"" << full_trait << "\"}\n\n";
            put_in_dict_file<typename T::MapType::TailType::KeySet>(dict_file);
        }
         
    }
}
   

int main()
{
    using namespace container;
    using namespace context;

    typedef typename RootModule::template ImplFor<PublicTrait>::type PublicTraitImplMap;
    typedef typename PublicTraitImplMap::KeySet PublicTraitImplSet;

    std::cout << "The set of publically-advertised traits is: "
              << container::repr::type_name<PublicTraitImplSet>()
              << std::endl;
              
    std::fstream dict_file;
    dict_file.open("trait_dict.py", std::ios::out);
    put_in_dict_file<PublicTraitImplSet>(dict_file);
    return 0;


    /*
    typedef TypeMap<Binding<key::RootModule, RootModule>> BaseInputState;

typedef typename BaseInputState
    ::template SetItem<key::RequirementSet, 
TypeSet<AddOne, AddIt, SubArgs, IsTrue, FFIEntry<AddOne>, FFIEntry<AddIt>, FFIEntry<SubArgs>, FFIEntry<IsTrue>, Print<int>, FFIEntry<Print<int>>, Print<float>, FFIEntry<Print<float>>, FFIGen>
			>::type StandardTraits;
        run<typename context::CreateContextType<StandardTraits>::type>();
    */
}
    