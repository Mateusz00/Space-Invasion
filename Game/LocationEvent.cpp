#include "LocationEvent.hpp"
#include "World.hpp"
#include <ios>

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
        case Event::SetScrolling:
        {
            auto speed = mParameters.find("scrollSpeed");

            if(speed != mParameters.end())
                world.setScrollingSpeed(std::stof(speed->second));
            else
                world.setScrollingSpeed(0.f);

            break;
        }
        case Event::ShowScore:
        {
            auto flag = mParameters.find("showScore");

            if(flag != mParameters.end())
            {
                bool isVisible;
                std::istringstream(flag->second) >> std::boolalpha >> isVisible;
                world.showScore(isVisible);
            }

            break;
        }
    }
}

void LocationEvent::addParameter(std::string key, std::string value)
{
    mParameters.emplace(std::move(key), std::move(value));
}
