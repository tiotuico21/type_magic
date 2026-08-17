
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "logic.h"

using RootModule = context::ModuleBundle<AddOneModule, AddItModule, PrintModule,FFIGenModule>;
;

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
    typedef TypeMap<Binding<key::RootModule, RootModule>> BaseInputState;

typedef typename BaseInputState
    ::template SetItem<key::RequirementSet, 
TypeSet<AddOne, AddIt, FFIEntry<AddOne>, FFIEntry<AddIt>, FFIGen>
			>::type StandardTraits;
        run<typename context::CreateContextType<StandardTraits>::type>();
        return 0;
    }
    