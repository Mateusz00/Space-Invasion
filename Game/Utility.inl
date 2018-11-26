template <typename T>
void centerOrigin(T& object)
{
    auto bounds = object.getLocalBounds();
    object.setOrigin(bounds.width / 2, bounds.height / 2);
}

template <typename T>
std::string toString(const T& value)
{
    std::ostringstream text;
    text << value;
    return text.str();
}
