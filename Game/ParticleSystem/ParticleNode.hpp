#ifndef PARTICLENODE_HPP
#define PARTICLENODE_HPP

#include "../SceneNode.hpp"
#include "Particle.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

class ParticleNode : public SceneNode
{
    public:
        void            addParticle(Particle::Type, sf::Vector2f position);

    private:
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        std::vector<Particle>   mParticles;
        sf::VertexArray         mVertices;
};

#endif // PARTICLENODE_HPP
