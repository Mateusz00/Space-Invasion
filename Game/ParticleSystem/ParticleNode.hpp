#ifndef PARTICLENODE_HPP
#define PARTICLENODE_HPP

#include "../SceneNode.hpp"
#include "../ResourcesID.hpp"
#include "Particle.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

class ParticleNode : public SceneNode
{
    public:
                        ParticleNode(const TextureHolder&);
        void            addParticle(Particle::Type, sf::Vector2f position);

    private:
        void            computeVertices() const;
        void            addVertex(float worldX, float worldY, float textureX, float textureY, const sf::Color&) const;
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        std::vector<Particle>       mParticles;
        mutable sf::VertexArray     mVertices;
        mutable bool                mNeedsUpdate;
        const sf::Texture&		    mTexture;
};

#endif // PARTICLENODE_HPP
