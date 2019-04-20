#include "Bar.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/Color.hpp>
#include <exception>

Bar::Bar(float val, float maxValue, sf::Vector2f maxSize)
    : mBar(maxSize),
      mMaxValue(maxValue),
      mCurrentValue(val),
      mMaxSize(maxSize)
{
    centerOrigin(mBar);
    if(maxValue <= 0)
        throw std::logic_error("Error: max value can't be zero or less (Bar)");
}

void Bar::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mCurrentValue > 0)
        target.draw(mBar, states);
}

void Bar::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    float percent = mCurrentValue / mMaxValue;
    percent = (percent > 1.f) ? 1.f : percent; // Stops Bar from enlarging more than it's initial size
    percent = (percent < 0.f) ? 0.f : percent; // Must be a positive value

    // Shrink and change color depending on percent
    sf::Uint8 newR = mMaxColor.r - mColorRange[0] * (1-percent);
    sf::Uint8 newG = mMaxColor.g - mColorRange[1] * (1-percent);
    sf::Uint8 newB = mMaxColor.b - mColorRange[2] * (1-percent);
    sf::Uint8 newA = mMaxColor.a - mColorRange[3] * (1-percent);
    mBar.setFillColor(sf::Color(newR, newG, newB, newA));

    mBar.setSize(sf::Vector2f(mMaxSize.x * percent, mMaxSize.y));
    centerOrigin(mBar);
}

void Bar::setPosition(sf::Vector2f position)
{
    mBar.setPosition(position);
}

void Bar::setPosition(float x, float y)
{
    mBar.setPosition(x, y);
}

void Bar::updateValue(float val)
{
    mCurrentValue = val;
}

///@param minColor - color of bar when current value is minimum
///@param maxColor - color of bar when current value is maximum
void Bar::setColorRange(sf::Color maxColor, sf::Color minColor)
{
    mColorRange[0] = static_cast<int>(maxColor.r) - static_cast<int>(minColor.r);
    mColorRange[1] = static_cast<int>(maxColor.g) - static_cast<int>(minColor.g);
    mColorRange[2] = static_cast<int>(maxColor.b) - static_cast<int>(minColor.b);
    mColorRange[3] = static_cast<int>(maxColor.a) - static_cast<int>(minColor.a);
    mMaxColor = maxColor;
}
