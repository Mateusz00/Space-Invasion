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

template <typename Obj, typename Txt>
void centerText(const Obj& object, Txt& text)
{
    auto bounds = object.getGlobalBounds();
    text.setPosition(sf::Vector2f(bounds.width  * 0.5f, bounds.height * 0.4f));
    centerOrigin(text);
}
