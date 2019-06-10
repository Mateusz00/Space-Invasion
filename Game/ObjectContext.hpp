#ifndef OBJECT_CONTEXT_HPP
#define OBJECT_CONTEXT_HPP

#include <vector>
#include "LocationEvent.hpp"
class SoundPlayer;
class ParticleNode;
class CommandQueue;

struct ObjectContext
{
    SoundPlayer*                    soundPlayer = nullptr;
    ParticleNode*                   particleNode = nullptr;
    CommandQueue*                   commandQueue = nullptr;
    std::vector<LocationEvent>*     events = nullptr;
};

#endif // OBJECT_CONTEXT_HPP
