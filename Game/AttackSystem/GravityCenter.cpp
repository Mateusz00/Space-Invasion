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

void GravityCenter::setDirection(sf::Vector2f dir)
{
    mDirection = dir;
}

sf::Vector2f GravityCenter::getDirection() const
/// Returns initial direction of projectile
{
    return mDirection;
}

void GravityCenter::setStartPos(sf::Vector2f pos)
{
    mStartPos = pos;
}

sf::Vector2f GravityCenter::getStartPos() const
{
    return mStartPos;
}

float GravityCenter::activeTime() const
/// Returns seconds that elapsed since creation of this gravityCenter
{
    return mTimeActive.asSeconds();
}

void GravityCenter::updateTime(sf::Time dt)
{
    mTimeActive += dt;
}
