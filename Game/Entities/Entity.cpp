#include "Entity.hpp"

Entity::Entity(int hitpoints)
    : mHitpoints(hitpoints)
{
}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::setHitpoints(int hitpoints)
{
    mHitpoints = hitpoints;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float x, float y)
{
    mVelocity.x = x;
    mVelocity.y = y;
}

void Entity::updateCurrent(sf::Time dt)
{
    move(mVelocity * dt.asSeconds());
}
