
template <typename TYPE>
static std::string query(){
    return __PRETTY_FUNCTION__;
}

template <typename TYPE>
static std::string get_type_name(){
    std::string voidPRETTY = query<void>();
    std::string ourPRETTY = query<TYPE>();

    int typeIndex = voidPRETTY.find("void");

    int difference = ourPRETTY.length() - voidPRETTY.length();

    int totalLength = difference + 4;

    return ourPRETTY.substr(typeIndex, totalLength);

}

