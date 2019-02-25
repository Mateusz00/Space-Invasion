#include "Entity.hpp"
#include "../World.hpp"
#include <iostream>
using ListIterator = std::list<Entity*>::iterator;


Entity::Entity(int hitpoints, bool isCollidable, World& world)
    : mHitpoints(hitpoints),
      mWorld(world)
{
    if(isCollidable)
        mWorld.addCollidable(this);
}

Entity::~Entity()
{
    std::cout << "Deleting Collidable!" << std::endl;
    mWorld.removeCollidable(this);
}

Entity& Entity::operator=(const Entity& other)
{
    setVelocity(other.getVelocity());
    setHitpoints(other.getHitpoints());
    mPosition = other.getPositionOnList();
    return *this;
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
    setVelocity(sf::Vector2f(x, y));
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

void Entity::destroy()
{
    mHitpoints = 0;
}

ListIterator Entity::getPositionOnList() const
{
    return mPosition;
}

World& Entity::getWorld() const
{
    return mWorld;
}

sf::FloatRect Entity::getLocalBounds() const
{
    return sf::FloatRect();
}

void Entity::onCollision(Entity& entity)
{
}

void Entity::removeEntity()
{
    destroy();
}

bool Entity::isMarkedForRemoval() const
{
    if(mHitpoints <= 0)
        return true;

    return false;
}

bool collision(const Entity& lhs, const Entity& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
