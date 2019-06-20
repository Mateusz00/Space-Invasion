#ifndef OBJECT_CONTEXT_HPP
#define OBJECT_CONTEXT_HPP

class SoundPlayer;
class ParticleNode;
class CommandQueue;
class EventQueue;

struct ObjectContext
{
    SoundPlayer*    soundPlayer = nullptr;
    ParticleNode*   particleNode = nullptr;
    CommandQueue*   commandQueue = nullptr;
    EventQueue*     events = nullptr;
};

#endif // OBJECT_CONTEXT_HPP
