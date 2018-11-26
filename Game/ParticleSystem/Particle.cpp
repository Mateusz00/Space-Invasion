#include "Particle.hpp"
#include "../DataTable.hpp"
#include <algorithm>

namespace
{
    const std::vector<ParticleData> table = initializeParticleData();
}

Particle::Particle(sf::Vector2f pos, Type type)
    : mType(type),
      mLifespan(table[mType].lifespan),
      mPosition(pos),
      mColor(table[mType].color)
{
}

void Particle::update(sf::Time dt)
{
    mLifespan -= dt;
    float timeRatio = mLifespan.asSeconds() / table[mType].lifespan.asSeconds();
    mColor.a = static_cast<sf::Uint8>(255 * std::max(timeRatio, 0.f)); // std::max to ensure positive result
    mPosition += mVelocity * dt.asSeconds();
}

void Particle::changeVelocity(sf::Vector2f vel)
{
    mVelocity = vel;
}

sf::Color Particle::getColor() const
{
    return mColor;
}

sf::Vector2f Particle::getPosition() const
{
    return mPosition;
}

sf::Time Particle::getLifespan() const
{
    return mLifespan;
}
