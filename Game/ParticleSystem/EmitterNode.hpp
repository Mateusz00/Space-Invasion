#ifndef EMITTERNODE_HPP
#define EMITTERNODE_HPP

#include "../SceneNode.hpp"
#include "ParticleNode.hpp"
#include "Particle.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

class EmitterNode : public SceneNode
{
    public:
                        EmitterNode(Particle::Type, ParticleNode&, float rate);

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;

        ParticleNode&   mParticleNode;
        Particle::Type  mType;
        const float     mEmissionRate;
        sf::Time        mTimePassed;
};

#endif // EMITTERNODE_HPP
