#include "Attack.hpp"
#include "AttackPattern.hpp"
#include "Attacks.hpp"
#include "../Category.hpp"
#include "../Entities/Spaceship.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
#include "../Application.hpp"
#include <iostream>
using Attacks::attackData;

Attack::Attack(int id, const TextureHolder& textures, sf::Vector2f pos, ObjectContext context, int shooterID,
                bool isAllied, const std::vector<Spaceship*>& targets, int phase)
    : Entity(1, false, context),
      mTargets(targets),
      mAttackID(id),
      mTextures(textures),
      mIsActive(true),
      mIsReadyToDelete(false),
      mIsAllied(isAllied),
      mPosition(pos),
      mShooterID(shooterID)
{
    addCategories(Category::Attack);
    createGravityCenters(phase);
    createProjectiles(phase);
}

void Attack::updateCurrent(sf::Time dt, CommandQueue& commandQueue)
{
    // Remove projectiles that are marked for removal
    auto iter = std::remove_if(mProjectiles.begin(), mProjectiles.end(), std::mem_fn(&Projectile::isMarkedForRemoval));
    mProjectiles.erase(iter, mProjectiles.end());

    if(mProjectiles.empty())
        deactivate();

    // Update gravityCenters positions
    for(auto& gravityCenterPair : mGravityCenters)
    {
        int gravityCenterID = gravityCenterPair.first;
        GravityCenter& gravityCenter = gravityCenterPair.second;
        gravityCenter.updateTime(dt);
        sf::Vector2f displacement;

        switch(gravityCenter.getPatternID())
        {
            case AttackPattern::Guided:
            {
                sf::Vector2f newVelocity = unitVector(dt.asSeconds() * getClosestTarget(&gravityCenter) * 170.f + gravityCenter.getVelocity()); // getVelocity and 170.f makes movement smoother
                newVelocity *= gravityCenter.getSpeed();
                gravityCenter.setVelocity(newVelocity);
                break;
            }
            case AttackPattern::Wave:
            {
                float angle = std::atan2(-gravityCenter.getDirection().y, gravityCenter.getDirection().x) + toRadian(90.f);
                sf::Vector2f startPos(gravityCenter.getStartPos());
                sf::Vector2f oldPos = gravityCenter.getPosition();

                sf::Vector2f newPos1(startPos.x + gravityCenter.getSpeed() * std::sin(angle) * gravityCenter.activeTime(),
                                     startPos.y + gravityCenter.getSpeed() * std::cos(angle) * gravityCenter.activeTime());

                float times = vectorLength(startPos - newPos1) / gravityCenter.getPatternData().waveData[1];
                float perpendicularLength = (std::sin(times * 3.1415f) * gravityCenter.getPatternData().waveData[0]);

                sf::Vector2f newPos2(perpendicularLength * std::sin(angle + toRadian(90.f)),
                                     perpendicularLength * std::cos(angle + toRadian(90.f)));

                // getTimePerFrame nullifies effect of multiplying by time_per_frame in entity's update func
                gravityCenter.setVelocity((newPos1 + newPos2 - oldPos) / Application::getTimePerFrame().asSeconds());
                break;
            }
            case AttackPattern::Orbiting:
            {
                // Apply angular displacement
                sf::Vector2f centerPos = mGravityCenters.at(gravityCenter.getPatternData().gravityCenterID).getPosition();
                sf::Vector2f oldPos = gravityCenter.getPosition();
                sf::Vector2f dir = oldPos - centerPos;
                sf::Vector2f newPosition;

                float radius = vectorLength(dir);
                float newAngle = toDegree(std::atan2(dir.y, dir.x)) + 90.f + gravityCenter.getSpeed() * dt.asSeconds();
                newPosition.x = centerPos.x + std::sin(toRadian(newAngle)) * radius;
                newPosition.y = centerPos.y - std::cos(toRadian(newAngle)) * radius;
                gravityCenter.setVelocity((newPosition - oldPos) / Application::getTimePerFrame().asSeconds());
                break;
            }
            case AttackPattern::Barrier:
            {
                // Center's position is updated during attack manager's update, apply only angular displacement
                sf::Vector2f oldPos = gravityCenter.getPosition();
                sf::Vector2f dir = oldPos - mPosition;
                sf::Vector2f newPosition;

                float radius = vectorLength(dir);
                float newAngle = toDegree(std::atan2(dir.y, dir.x)) + 90.f + gravityCenter.getSpeed() * dt.asSeconds();
                newPosition.x = mPosition.x + std::sin(toRadian(newAngle)) * radius;
                newPosition.y = mPosition.y - std::cos(toRadian(newAngle)) * radius;
                gravityCenter.setVelocity((newPosition - oldPos) / Application::getTimePerFrame().asSeconds());
                break;
            }
        }

        // Apply displacement vector to gravity center and it's projectiles and gravityCenters
        displacement = gravityCenter.getVelocity() * dt.asSeconds();
        gravityCenter.update(dt, commandQueue);
        applyDisplacement(gravityCenterID, displacement);
    }

    // Update projectiles positions
    for(auto& projectile : mProjectiles)
    {
        projectile->updateTime(dt);

        switch(static_cast<AttackPattern::ID>(projectile->getPattern()))
        {
            case AttackPattern::Guided:
            {
                sf::Vector2f newVel = unitVector(dt.asSeconds() * getClosestTarget(projectile.get()) * 170.f + projectile->getVelocity()); // getVelocity and 170.f makes movement more parabolic
                newVel *= projectile->getMaxSpeed();
                projectile->setVelocity(newVel);
                break;
            }
            case AttackPattern::Wave:
            {
                float angle = std::atan2(-projectile->getDirection().y, projectile->getDirection().x) + toRadian(90.f);
                sf::Vector2f startPos(projectile->getStartPos());

                sf::Vector2f newPos1(startPos.x + projectile->getMaxSpeed() * std::sin(angle) * projectile->activeTime(),
                                     startPos.y + projectile->getMaxSpeed() * std::cos(angle) * projectile->activeTime());

                float times = vectorLength(startPos - newPos1) / projectile->getPatternData().waveData[1];
                float perpendicularLength = (std::sin(times * 3.1415f) * projectile->getPatternData().waveData[0]);

                sf::Vector2f newPos2(perpendicularLength * std::sin(angle + toRadian(90.f)),
                                     perpendicularLength * std::cos(angle + toRadian(90.f)));

                // getTimePerFrame nullifies effect of multiplying by time_per_frame in entity's update func
                float seconds = Application::getTimePerFrame().asSeconds();
                projectile->setVelocity((newPos1 + newPos2 - projectile->getPosition()) / seconds);
                break;
            }
            case AttackPattern::Orbiting:
            {
                // Apply angular displacement
                sf::Vector2f centerPos = mGravityCenters.at(projectile->getPatternData().gravityCenterID).getPosition();
                sf::Vector2f dir = projectile->getWorldPosition() - centerPos;
                float radius = vectorLength(dir);
                float newAngle = toDegree(std::atan2(dir.y, dir.x)) + 90.f + projectile->getMaxSpeed() * dt.asSeconds();
                sf::Vector2f newPosition;
                newPosition.x = centerPos.x + std::sin(toRadian(newAngle)) * radius;
                newPosition.y = centerPos.y - std::cos(toRadian(newAngle)) * radius;

                // getTimePerFrame nullifies effect of multiplying by time_per_frame in entity's update func
                float seconds = Application::getTimePerFrame().asSeconds();
                projectile->setVelocity((newPosition - projectile->getPosition()) / seconds);
                break;
            }
            case AttackPattern::Barrier:
            {
                // Center's position is updated during attack manager's update
                sf::Vector2f dir = projectile->getWorldPosition() - mPosition;
                float radius = vectorLength(dir);
                float newAngle = toDegree(std::atan2(dir.y, dir.x)) + 90.f + projectile->getMaxSpeed() * dt.asSeconds();
                sf::Vector2f newPosition;
                newPosition.x = mPosition.x + std::sin(toRadian(newAngle)) * radius;
                newPosition.y = mPosition.y - std::cos(toRadian(newAngle)) * radius;

                // getTimePerFrame nullifies effect of multiplying by time_per_frame in entity's update func
                float seconds = Application::getTimePerFrame().asSeconds();
                projectile->setVelocity((newPosition - projectile->getPosition()) / seconds);
                break;
            }
        }
    }

    for(auto& projectile : mProjectiles)
        projectile->update(dt, commandQueue);
}

void Attack::createProjectiles(int phase)
{
    const auto projectileNumber = attackData.at(mAttackID).phases[phase].projectiles.size();

    for(int i=0; i < projectileNumber; ++i)
        createProjectile(phase, i);
}

void Attack::createProjectile(int phase, int projectileNum)
{
    const auto& projectileInfo = attackData.at(mAttackID).phases[phase].projectiles[projectileNum];
    Projectiles::ID type = projectileInfo.type;
    float speed = projectileInfo.speed;
    int sign = (isAllied()) ? -1 : 1;

    std::unique_ptr<Projectile> projectile(new Projectile(type, mTextures, getObjectContext(), mShooterID, speed, !isAllied()));
    projectile->setPattern(projectileInfo.pattern);
    projectile->setPatternData(projectileInfo.patternData);

    sf::Vector2f direction;
    sf::Vector2f offset(projectileInfo.offset.x , projectileInfo.offset.y * sign);

    if(projectile->getPattern() == AttackPattern::Orbiting)
    {
        try
        {
            projectile->setPosition(mGravityCenters.at(projectileInfo.patternData.gravityCenterID).getPosition() + offset);
        }
        catch(const std::out_of_range& oor)
        {
            std::cout << "Could not find gravityCenter(id=" << projectileInfo.patternData.gravityCenterID
                      << ") in (AttackID=" << mAttackID << ", projectileNumber=" << projectileNum << ")" << std::endl;
            throw oor;
        }
    }
    else
        projectile->setPosition(mPosition + offset);

    sf::Vector2f targetVector;
    if(projectileInfo.isAimed && (targetVector = getClosestTarget(projectile.get())) != sf::Vector2f())
        direction = unitVector(targetVector);
    else
    {
        float radians = toRadian(projectileInfo.direction);
        direction = sf::Vector2f(1 * std::sin(radians), 1 * std::cos(radians) * sign);
    }

    sf::Vector2f velocity(direction * speed);
    projectile->setVelocity(velocity);
    projectile->setDirection(direction);
    projectile->setStartPos(projectile->getPosition());

    mProjectiles.emplace_back(std::move(projectile));
}

void Attack::createGravityCenters(int phase)
{
    const auto gravityCentersNumber = attackData.at(mAttackID).phases[phase].gravityCenters.size();

    for(int i=0; i < gravityCentersNumber; ++i)
        createGravityCenter(phase, i);
}

void Attack::createGravityCenter(int phase, int centerNum)
{
    const auto& gravityCenterData = attackData.at(mAttackID).phases[phase].gravityCenters[centerNum];
    GravityCenter gravityCenter(gravityCenterData.speed, gravityCenterData.pattern, gravityCenterData.patternData);
    int sign = (isAllied()) ? -1 : 1;

    sf::Vector2f direction;
    sf::Vector2f offset(gravityCenterData.offset.x, gravityCenterData.offset.y * sign);

    if(gravityCenter.getPatternID() == AttackPattern::Orbiting)
        gravityCenter.setPosition(mGravityCenters.at(gravityCenter.getPatternData().gravityCenterID).getPosition() + offset);
    else
        gravityCenter.setPosition(mPosition + offset);

    sf::Vector2f targetVector;
    if(gravityCenterData.isAimed && (targetVector = getClosestTarget(&gravityCenter)) != sf::Vector2f())
        direction = unitVector(targetVector);
    else
    {
        float radians = toRadian(gravityCenterData.direction);
        direction = sf::Vector2f(1 * std::sin(radians), 1 * std::cos(radians) * sign);
    }

    sf::Vector2f velocity(direction * gravityCenterData.speed);
    gravityCenter.setVelocity(velocity);
    gravityCenter.setDirection(direction);
    gravityCenter.setStartPos(gravityCenter.getPosition());

    mGravityCenters.emplace(gravityCenterData.id, std::move(gravityCenter));
}

void Attack::activate()
{
    mIsActive = true;
}

void Attack::deactivate()
{
    mIsActive = false;
}

bool Attack::isActive() const
{
    return mIsActive;
}

sf::FloatRect Attack::getBoundingRect() const
{
    float maxTop, maxBottom, maxLeft, maxRight;

    maxTop      = std::numeric_limits<float>::max();
    maxBottom   = std::numeric_limits<float>::min();
    maxLeft     = std::numeric_limits<float>::max();
    maxRight    = std::numeric_limits<float>::min();

    for(const auto& projectile : mProjectiles)
    {
        maxTop      = std::min(maxTop, projectile->getBoundingRect().top);
        maxBottom   = std::max(maxBottom, projectile->getBoundingRect().top + projectile->getBoundingRect().height);
        maxLeft     = std::min(maxLeft, projectile->getBoundingRect().left);
        maxRight    = std::max(maxRight, projectile->getBoundingRect().left + projectile->getBoundingRect().width);
    }

    return sf::FloatRect(maxLeft, maxTop, maxRight - maxLeft, maxBottom - maxTop); // Rect containing all projectiles
}

void Attack::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto& projectile : mProjectiles)
        target.draw(*projectile, states);
}

void Attack::removeEntity()
{
    mProjectiles.clear();
    deactivate();
}

bool Attack::isMarkedForRemoval() const
{
    if(mProjectiles.empty() && mIsReadyToDelete)
        return true;
    else
        return false;
}

void Attack::markForRemoval()
{
    mIsReadyToDelete = true;
}

void Attack::updateBarrierPosition(sf::Vector2f displacement)
{
    bool isBarrier = false;

    for(auto& gravityCenter : mGravityCenters)
    {
        if(gravityCenter.second.getPatternID() == AttackPattern::Barrier)
        {
            isBarrier = true;
            gravityCenter.second.move(displacement);
        }
    }
    for(auto& projectile : mProjectiles)
    {
        if(projectile->getPattern() == AttackPattern::Barrier)
        {
            isBarrier = true;
            projectile->move(displacement);
        }
    }

    if(isBarrier)
        mPosition = mPosition + displacement;
}

void Attack::applyDisplacement(int gravityCenterID, sf::Vector2f displacement)
{
    std::vector<int> changedCenters;

    for(auto& gravityCenter : mGravityCenters)
    {
        if(gravityCenter.second.getPatternID() == AttackPattern::Orbiting)
            if(gravityCenter.second.getPatternData().gravityCenterID == gravityCenterID)
            {
                gravityCenter.second.move(displacement);
                changedCenters.push_back(gravityCenter.first);
            }
    }

    for(auto& projectile : mProjectiles)
    {
        if(projectile->getPattern() == AttackPattern::Orbiting)
            if(projectile->getPatternData().gravityCenterID == gravityCenterID)
                projectile->move(displacement);
    }

    for(int id : changedCenters)
        applyDisplacement(id, displacement);
}

///@return normalized direction vector
sf::Vector2f Attack::getClosestTarget(const sf::Transformable* object) const
{
    float smallestDistance = std::numeric_limits<float>::max();
    Spaceship* closestTarget = nullptr;

    for(const auto& target : mTargets)
    {
        bool isTargetBehind;
        if(isAllied())
            isTargetBehind = target->getWorldPosition().y < object->getPosition().y; // Avoids projectile/gravityCenter turning back
        else
            isTargetBehind = target->getWorldPosition().y > object->getPosition().y; // Avoids projectile/gravityCenter turning back

        if(isTargetBehind)
        {
            float targetDistance = vectorLength(object->getPosition() - target->getWorldPosition());
            if(targetDistance < smallestDistance)
            {
                closestTarget = target;
                smallestDistance = targetDistance;
            }
        }
    }

    if(closestTarget)
        return unitVector(closestTarget->getWorldPosition() - object->getPosition());
    else
        return sf::Vector2f();
}

bool Attack::isAllied() const
{
    return mIsAllied;
}
