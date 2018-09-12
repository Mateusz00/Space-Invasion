#include "Utility.hpp"

void centerOrigin(sf::Sprite& sprite)
{
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void centerOrigin(sf::Text& text)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2, bounds.height / 2);
}
