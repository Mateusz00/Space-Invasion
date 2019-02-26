#include "GravityCenter.hpp"

GravityCenter::GravityCenter(float speed, AttackPattern::ID patternID, AttackPattern::PatternData patternData)
    : mSpeed(speed),
      mPattern(patternID),
      mPatternData(patternData)
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

float GravityCenter::getSpeed() const
{
    return mSpeed;
}

void GravityCenter::setSpeed(float speed)
{
    mSpeed = speed;
}

AttackPattern::ID GravityCenter::getPatternID() const
{
    return mPattern;
}

AttackPattern::PatternData GravityCenter::getPatternData() const
{
    return mPatternData;
}
