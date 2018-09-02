template <typename T>
std::string toString(const T& value)
{
    std::ostringstream text;
    text << value;
    return text.str();
}

