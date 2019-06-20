#ifndef EVENTQUEUE_HPP
#define EVENTQUEUE_HPP

#include "LocationEvent.hpp"
#include <vector>

class EventQueue
{
    public:
        void                push(LocationEvent);
        LocationEvent       pop();
        LocationEvent&      front();
        bool                isEmpty() const;

    private:
        void sortLocationEvents();

        std::vector<LocationEvent> mEvents;
};

#endif // EVENTQUEUE_HPP
