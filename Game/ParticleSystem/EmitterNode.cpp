#include "EmitterNode.hpp"

EmitterNode::EmitterNode(Particle::Type type, ParticleNode& node, float rate)
    : mParticleNode(node),
      mType(type),
      mEmissionRate(rate)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue)
{
    const sf::Time interval = sf::seconds(1.f) / mEmissionRate;
    mTimePassed += dt;

    if(mTimePassed >= interval)
    {
        mTimePassed -= interval;
        mParticleNode.addParticle(mType, getWorldPosition());
    }
}
