#include "GUISlider.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

GUISlider::GUISlider(float width, float height, float currentValue, FontHolder& fonts, GUIContainer* container)
    : GUISlider(sf::Vector2f(width, height), currentValue, fonts, container)
{
}

GUISlider::GUISlider(sf::Vector2f sliderSize, float currentValue, FontHolder& fonts, GUIContainer* container)
    : mSlider(sf::Vector2f(std::min(sliderSize.x * 0.1f, 15.f), sliderSize.y)),
      mBar(sf::Vector2f(sliderSize.x, sliderSize.y - 4.f)),
      mValue(currentValue),
      mText(toString(static_cast<int>(mValue)), fonts.get(Fonts::Sansation), static_cast<unsigned int>(sliderSize.y * 0.8f)),
      mContainer(container),
      mShowValue(true)
{
    mText.setPosition(sliderSize.x + 20.f, 0.f);

    centerOrigin(mSlider);
    mSlider.setFillColor(sf::Color(96, 38, 38));
    mSlider.setPosition(getBoundingRect().width * mValue / 100.f , mBar.getLocalBounds().height * 0.5f);

    mBar.setFillColor(sf::Color(58, 27, 27));
    mBar.setOutlineColor(sf::Color(96, 38, 38));
    mBar.setOutlineThickness(1.f);
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
    sf::FloatRect boundingRect;

    if(mContainer)
        boundingRect = mContainer->getTransform().transformRect(getTransform().transformRect(mBar.getGlobalBounds()));
    else // if nullptr
        boundingRect = getTransform().transformRect(mBar.getGlobalBounds());

    float distance = static_cast<float>(mouseXPosition) - boundingRect.left;

    // Prevent slider from leaving bar
    float percent = std::min(1.f, distance / mBar.getLocalBounds().width);
    percent = std::max(0.f, percent);

    // Assign new value and update text
    mValue = (mMaxValue - mMinValue) * percent;
    mText.setString(toString(static_cast<int>(mValue)));
    mSlider.setPosition(mBar.getLocalBounds().width * percent, mSlider.getPosition().y);

    mUpdatingFunction(mValue);
}

void GUISlider::setUpdatingFunction(std::function<void(float)> func)
{
    mUpdatingFunction = func;
}

void GUISlider::setValueRange(float minValue, float maxValue)
{
    mMinValue = minValue;
    mMaxValue = maxValue;
}

void GUISlider::setValueVisibility(bool showValue)
{
    mShowValue = showValue;
}

void GUISlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(mBar, states);
    target.draw(mSlider, states);

    if(mShowValue)
        target.draw(mText, states);
}
