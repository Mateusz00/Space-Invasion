#include "EventQueue.hpp"
#include <algorithm>

void EventQueue::push(LocationEvent event)
{
    mEvents.emplace_back(std::move(event));
    sortLocationEvents();
}

LocationEvent EventQueue::pop()
{
    LocationEvent event = mEvents.back();
    mEvents.pop_back();
    return event;
}

LocationEvent& EventQueue::front()
{
    return mEvents.back();
}

bool EventQueue::isEmpty() const
{
    return mEvents.empty();
}

void EventQueue::sortLocationEvents()
{
    std::sort(mEvents.begin(), mEvents.end(), [](LocationEvent& lhs, LocationEvent& rhs)
    {
        return lhs.getTriggerLocation().y < rhs.getTriggerLocation().y;
    });
}
