#include "GravityCenter.hpp"

GravityCenter::GravityCenter()
{
}

sf::Vector2f GravityCenter::getVelocity() const
{
    return mVelocity;
}

void GravityCenter::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void GravityCenter::setVelocity(float x, float y)
{
    setVelocity(sf::Vector2f(x, y));
}

void GravityCenter::update(sf::Time dt, CommandQueue&)
{
    move(mVelocity * dt.asSeconds());
}
