
std::string static cppToNumbaType(std::string cppType)
{
    static const std::unordered_map<std::string, std::string> typeMap = {
        {"bool", "types.boolean"},

        {"char", "types.int8"},
        {"signed char", "types.int8"},
        {"unsigned char", "types.uint8"},

        {"short", "types.int16"},
        {"unsigned short", "types.uint16"},

        {"int", "types.int32"},
        {"unsigned", "types.uint32"},
        {"unsigned int", "types.uint32"},

        {"long", "types.int64"},
        {"unsigned long", "types.uint64"},

        {"long long", "types.int64"},
        {"unsigned long long", "types.uint64"},

        {"int8_t", "types.int8"},
        {"uint8_t", "types.uint8"},
        {"int16_t", "types.int16"},
        {"uint16_t", "types.uint16"},
        {"int32_t", "types.int32"},
        {"uint32_t", "types.uint32"},
        {"int64_t", "types.int64"},
        {"uint64_t", "types.uint64"},

        {"float", "types.float32"},
        {"double", "types.float64"},
        {"std::string",
         "CPointer(types.int8)"},
        {"std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
         "CPointer(types.int8)"},
        {"void", "types.void"}};

    // Remove trailing whitespace
    while (!cppType.empty() && std::isspace(cppType.back()))
        cppType.pop_back();

    int pointerDepth = 0;

    // Count trailing '*'
    while (!cppType.empty() && cppType.back() == '*')
    {
        ++pointerDepth;
        cppType.pop_back();

        while (!cppType.empty() && std::isspace(cppType.back()))
            cppType.pop_back();
    }

    // Special case: void*
    if (cppType == "void" && pointerDepth == 1)
        return "types.voidptr";

    auto it = typeMap.find(cppType);

    if (it == typeMap.end())
        throw std::runtime_error("Unknown C++ type: " + cppType);

    std::string result = it->second;

    for (int i = 0; i < pointerDepth; ++i)
    {
        result = "types.CPointer(" + result + ")";
    }

    return result;
}

