#include "GUISlider.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

GUISlider::GUISlider(sf::Vector2f sliderSize, float minValue, float maxValue, float& out, FontHolder& fonts)
    : mSlider(sf::Vector2f(std::min(sliderSize.x * 0.1f, 15.f), sliderSize.y)),
      mBar(sf::Vector2f(sliderSize.x, sliderSize.y * 0.8f)),
      mValue(out),
      mMaxValue(maxValue),
      mMinValue(minValue),
      mText(toString(mValue), fonts.get(Fonts::Sansation), static_cast<unsigned int>(sliderSize.y * 0.6f))
{
    mText.setPosition(sliderSize.x + 20.f, sliderSize.y * 0.25f);

    centerOrigin(mSlider);
    mSlider.setFillColor(sf::Color(96, 38, 38));
    mSlider.setPosition(0.f, mBar.getLocalBounds().height * 0.5f);

    mBar.setFillColor(sf::Color(58, 27, 27));
    mBar.setOutlineColor(sf::Color(96, 38, 38));
    mBar.setOutlineThickness(1.f);
}

void GUISlider::activate()
{
}

void GUISlider::deactivate()
{
    GUIObject::deactivate();
    mSlider.setFillColor(sf::Color(96, 38, 38));
}

bool GUISlider::isSelectable() const
{
    return false;
}

void GUISlider::handleEvent(const sf::Event& event)
{
    if(isActive())
    {
        switch(event.type)
        {
            case sf::Event::MouseButtonReleased:
                deactivate();
                break;

            case sf::Event::MouseMoved:
                if(isActive())
                    changeSliderPosition(event.mouseMove.x);
                break;
        }
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        if(getBoundingRect().contains(event.mouseButton.x, event.mouseButton.y))
            onMouseClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    }
}

sf::FloatRect GUISlider::getBoundingRect() const
{
    sf::FloatRect bounds = mBar.getGlobalBounds();
    bounds.top = mSlider.getLocalBounds().top;
    bounds.height = mSlider.getLocalBounds().height;

    return getTransform().transformRect(bounds);
}

void GUISlider::onMouseClick(sf::Vector2i mousePosition)
{
    GUIObject::activate();
    mSlider.setFillColor(sf::Color(132, 42, 42));
    changeSliderPosition(mousePosition.x);
}

void GUISlider::changeSliderPosition(int mouseXPosition)
{
    float distance = static_cast<float>(mouseXPosition) - getTransform().transformRect(mBar.getGlobalBounds()).left;

    // Prevent slider from leaving bar
    float percent = std::min(1.f, distance / mBar.getLocalBounds().width);
    percent = std::max(0.f, percent);

    mValue = (mMaxValue - mMinValue) * percent;
    mText.setString(toString(mValue));
    mSlider.setPosition(mBar.getLocalBounds().width * percent, mSlider.getPosition().y);
}

void GUISlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(mBar, states);
    target.draw(mSlider, states);
    target.draw(mText, states);
}
