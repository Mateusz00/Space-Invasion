#include "GUILabel.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

GUILabel::GUILabel(const std::string& text, const FontHolder& fonts)
    : mText(text, fonts.get(Fonts::Sansation), 20u)
{
}

void GUILabel::setText(const std::string& text)
{
	mText.setString(text);
}

bool GUILabel::isSelectable() const
{
    return false;
}

void GUILabel::handleEvent(const sf::Event& event)
{
}

void GUILabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mText, states);
}

sf::FloatRect GUILabel::getBoundingRect() const
{
    return getTransform().transformRect(mText.getGlobalBounds());
}
