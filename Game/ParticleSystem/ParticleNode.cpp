#include "ParticleNode.hpp"
ParticleNode::ParticleNode(const TextureHolder& textures)
    : mVertices(sf::Quads),
      mNeedsUpdate(true),
      mTexture(textures.get(Textures::Particle))
{
}

void ParticleNode::addParticle(Particle::Type type, sf::Vector2f position)
{
    mParticles.emplace_back(Particle(position, type));
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue)
{
    if(!mParticles.empty())
    {
        auto beginning = std::remove_if(mParticles.begin(), mParticles.end(), [](Particle particle)
        {
            return (particle.getLifespan() <= sf::Time::Zero);
        });
        mParticles.erase(beginning, mParticles.end());
    }

    for(auto& particle : mParticles)
        particle.update(dt);

    mNeedsUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mNeedsUpdate)
    {
        computeVertices();
        mNeedsUpdate = false;
    }

    states.texture = &mTexture;
    target.draw(mVertices, states);
}

void ParticleNode::computeVertices() const
{
    sf::Vector2f textureSize(mTexture.getSize());
    sf::Vector2f halfTexture = textureSize / 2.f;
    mVertices.clear();

    for(const auto& particle : mParticles)
    {
        const sf::Vector2f& pos = particle.getPosition();
        const sf::Color& color = particle.getColor();

        addVertex(pos.x - halfTexture.x, pos.y - halfTexture.y, 0.f,           0.f,           color); // Left-top corner
        addVertex(pos.x + halfTexture.x, pos.y - halfTexture.y, textureSize.x, 0.f,           color); // Right-top corner
        addVertex(pos.x + halfTexture.x, pos.y + halfTexture.y, textureSize.x, textureSize.y, color); // Right-bottom corner
        addVertex(pos.x - halfTexture.x, pos.y + halfTexture.y, 0.f,           textureSize.y, color); // Left-bottom corner
    }
}

void ParticleNode::addVertex(float worldX, float worldY, float textureX, float textureY, const sf::Color& color) const
{
    sf::Vertex vertex(sf::Vector2f(worldX, worldY), color, sf::Vector2f(textureX, textureY));
    mVertices.append(vertex);
}
