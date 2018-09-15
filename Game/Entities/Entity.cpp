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

void Entity::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

void Entity::accelerate(float x, float y)
{
    mVelocity.x += x;
    mVelocity.y += y;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    move(mVelocity * dt.asSeconds());
}

void Entity::damage(int hitpoints)
{
    mHitpoints -= hitpoints;
}

void Entity::repair(int hitpoints)
{
    mHitpoints += hitpoints;
}

sf::FloatRect Entity::getLocalBounds() const
{
    return sf::FloatRect();
}
