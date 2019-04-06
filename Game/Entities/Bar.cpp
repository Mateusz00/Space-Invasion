#include "HealthBar.hpp"
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

    if(percent > 0) // Shrink and change color depending on percent
    {
        mBar.setFillColor(sf::Color(255*(1-percent), 255*percent, 0, 255));
        mBar.setSize(sf::Vector2f(mMaxSize.x * percent, mMaxSize.y));
        centerOrigin(mBar);
    }
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
