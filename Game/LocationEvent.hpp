#ifndef LOCATIONEVENT_HPP
#define LOCATIONEVENT_HPP

#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <string>
class World;

class LocationEvent
{
    public:
        enum Event{SetScrolling, ShowScore};

                        LocationEvent(sf::Vector2f, Event);
        sf::Vector2f    getTriggerLocation() const;
        void            handle(World&);
        void            addParameter(std::string key, std::string value);

    private:
        sf::Vector2f                                    mTriggerLocation;
        Event                                           mEventID;
        std::unordered_map<std::string, std::string>    mParameters;
};

#endif // LOCATIONEVENT_HPP
