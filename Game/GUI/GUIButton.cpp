#include "GUIButton.hpp"
#include <SFML/System/Vector2.hpp>

GUIButton::GUIButton()
{
    mSprite.setSize(sf::Vector2f(100.f, 100.f));
    mSprite.setFillColor(sf::Color::Magenta);
}

void GUIButton::select()
{
}

void GUIButton::deselect()
{
}

void GUIButton::activate()
{
    GUIObject::activate();
}

void GUIButton::deactivate()
{
    GUIObject::deactivate();
}

bool GUIButton::isSelectable() const
{
    return true;
}

void GUIButton::handleEvent(const sf::Event& event)
{
    // For toggled ones
}

sf::FloatRect GUIButton::getBoundingRect() const
{
    return getTransform().transformRect(mSprite.getGlobalBounds());
}

void GUIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}
