#include "HealthBar.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/Color.hpp>
#include <exception>

HealthBar::HealthBar(Entity& object, sf::Vector2f dimensions, int maxHitpoints)
    : mHealthBar(dimensions),
      mMaxValue(maxHitpoints),
      mMaxSize(mHealthBar.getSize()),
      mObject(object)
{
    centerOrigin(mHealthBar);
    if(maxHitpoints <= 0)
        throw std::logic_error("Error: Health can't be zero or less (healthbar)");
}

void HealthBar::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mObject.getHitpoints() > 0)
        target.draw(mHealthBar, states);
}

void HealthBar::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    float hitpointsPercent = mObject.getHitpoints() / mMaxValue;

    if(hitpointsPercent > 0) // Shrink and change color depending on hitpointsPercent
    {
        mHealthBar.setFillColor(sf::Color(255*(1-hitpointsPercent), 255*hitpointsPercent, 0, 255));
        mHealthBar.setSize(sf::Vector2f(mMaxSize.x * hitpointsPercent, mMaxSize.y));
        centerOrigin(mHealthBar);
    }
}

void HealthBar::setPosition(sf::Vector2f position)
{
    mHealthBar.setPosition(position);
}

void HealthBar::setPosition(float x, float y)
{
    mHealthBar.setPosition(x, y);
}
