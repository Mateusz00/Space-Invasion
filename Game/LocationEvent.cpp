#include "LocationEvent.hpp"
#include "World.hpp"

LocationEvent::LocationEvent(sf::Vector2f pos, Event id)
    : mTriggerLocation(pos),
      mEventID(id)
{
}

sf::Vector2f LocationEvent::getTriggerLocation() const
{
    return mTriggerLocation;
}

void LocationEvent::handle(World& world)
{
    switch(mEventID)
    {
        case Event::BossStage:
            world.setScrollingSpeed(0.f);///
        break;
    }
}

void LocationEvent::addParameter(std::string key, std::string value)
{
    mParameters.emplace(std::move(key), std::move(value));
}
