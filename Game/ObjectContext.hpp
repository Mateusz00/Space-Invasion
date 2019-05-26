#ifndef OBJECT_CONTEXT_HPP
#define OBJECT_CONTEXT_HPP

#include <vector>
class SoundPlayer;
class CommandQueue;

struct ObjectContext
{
    SoundPlayer*                    soundPlayer;
    CommandQueue*                   commandQueue;
    std::vector<LocationEvent>      events;
};

#endif // OBJECT_CONTEXT_HPP
