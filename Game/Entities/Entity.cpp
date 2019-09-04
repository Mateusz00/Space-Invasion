#include "Entity.hpp"
#include "../World.hpp"
int Entity::mNextFreeID = 0;

Entity::Entity(float hitpoints, bool isCollidable, ObjectContext context)
    : mHitpoints(hitpoints),
      mContext(context),
      mIsCollidable(isCollidable),
      mID(mNextFreeID++)
{
    if(isCollidable)
        addCategories(Category::Collidable);
}

void Entity::resetIDs()
{
    mNextFreeID = 0;
}

const sf::IntRect& Entity::getTextureRect() const
{

}

const sf::Texture* Entity::getTexture() const
{

}

float Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::setHitpoints(float hitpoints)
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

void Entity::damage(float hitpoints)
{
    if(hitpoints > 0)
        mHitpoints -= hitpoints;
}

void Entity::repair(float hitpoints)
{
    if(hitpoints > 0)
        mHitpoints += hitpoints;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

const ObjectContext& Entity::getObjectContext() const
{
    return mContext;
}

sf::FloatRect Entity::getLocalBounds() const
{
    return sf::FloatRect();
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

bool Entity::isCollidable() const
{
    return mIsCollidable;
}

bool collision(const Entity& lhs, const Entity& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

int Entity::getEntityID() const
{
    return mID;
}
