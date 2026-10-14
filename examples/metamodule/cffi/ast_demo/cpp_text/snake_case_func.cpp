
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


    static std::string toSnakeCaseMeta(const std::string& trait_name)
    {
        size_t start = trait_name.find('<');
        size_t end = trait_name.rfind('>');

        // No <...> found: just snake-case the whole name
        if (start == std::string::npos ||
            end == std::string::npos ||
            end <= start)
        {
            return trait_name;
        }

        // Get "Meta" and "Print"
        std::string prefix = trait_name.substr(0, start);
        std::string inner  = trait_name.substr(start + 1, end - start - 1);

        std::string result;

        // Add prefix: Meta -> meta
        for (char c : prefix)
        {
            result += static_cast<char>(
                std::tolower(static_cast<unsigned char>(c))
            );
        }

        // Add underscore between Meta and Print
        result += '_';

        // Convert inner to snake_case
        for (size_t i = 0; i < inner.size(); ++i)
        {
            unsigned char c = static_cast<unsigned char>(inner[i]);

            if (std::isupper(c))
            {
                // Add _ before uppercase letters, except the first character
                if (i != 0 && inner[i - 1] != '_')
                {
                    result += '_';
                }

                result += static_cast<char>(std::tolower(c));
            }
            else
            {
                result += inner[i];
            }
        }

        return result;
    }