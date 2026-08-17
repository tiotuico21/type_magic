
     static std::string toSnakeCase(std::string &trait_name)
    {
        std::string snake_case_trait_name;

        for (size_t i = 0; i < trait_name.size(); i++)
        {
            if (std::isupper(static_cast<unsigned char>(trait_name[i])) || static_cast<unsigned char>(trait_name[i]) == ':')
            {
                if (i != 0)
                {
                    snake_case_trait_name += '_';
                }
                if (static_cast<unsigned char>(trait_name[i]) != ':')
                {
                    snake_case_trait_name += std::tolower(static_cast<unsigned char>(trait_name[i]));
                }
            }
            else
            {
                snake_case_trait_name += trait_name[i];
            }
        }
        return snake_case_trait_name;
    }

