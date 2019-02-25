#include "Attack.hpp"
#include "AttackPattern.hpp"
#include "Attacks.hpp"
#include "../Category.hpp"
#include "../Entities/Aircraft.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
using Attacks::attackData;
///ADD createGravityCenters()
Attack::Attack(int id, const TextureHolder& textures, sf::Vector2f pos, World& world, int shooterID, Targets targets)
    : Entity(1, false, world),
      mPossibleTargets(targets),
      mAttackID(id),
      mTextures(textures),
      mIsActive(true),
      mIsReadyToDelete(false),
      mPosition(pos),
      mShooterID(shooterID)
{
    createProjectiles();
}

void Attack::update(sf::Time dt, CommandQueue& commandQueue)
{
    // Remove projectiles that are marked for removal
    auto iter = std::remove_if(mProjectiles.begin(), mProjectiles.end(), std::mem_fn(&Projectile::isMarkedForRemoval));
    mProjectiles.erase(iter, mProjectiles.end());

    // TODO: Remove gravityCenters without projectiles and without other gravity centers orbiting around it

    if(mProjectiles.empty())
        deactivate();

    // Update gravityCenters positions
    for(auto& gravityCenterPair : mGravityCenters)
    {
        int gravityCenterID = gravityCenterPair.first;
        GravityCenter& gravityCenter = gravityCenterPair.second;

        switch(gravityCenter.getPatternID())
        {
            case AttackPattern::Guided:
            {
                sf::Vector2f newVel = unitVector(dt.asSeconds() * getClosestTarget(&gravityCenter) * 170.f + gravityCenter.getVelocity()); // getVelocity and 170.f makes movement smoother
                newVel *= gravityCenter.getSpeed();
                projectile->setVelocity(newVel);
                break;
            }
            case AttackPattern::Spiral:
                break;
            case AttackPattern::Orbiting:
            {
                // Update center's position and apply angular displacement
                sf::Vector2f dir = gravityCenter.getPosition() /*- mPosition*/;
                float radius = vectorLength(dir);
                break;
            }
            case AttackPattern::Barrier:
            {
                // Center's position is updated during attack manager's update, apply only angular displacement
                sf::Vector2f dir = gravityCenter.getPosition() - mPosition;
                float radius = vectorLength(dir);
                float newAngle = toDegree(std::atan2(dir.y, dir.x)) + 90.f + gravityCenter.getSpeed() * dt.asSeconds();
                sf::Vector2f newPosition;
                newPosition.x = mPosition.x + std::sin(toRadian(newAngle)) * radius;
                newPosition.y = mPosition.y + -(std::cos(toRadian(newAngle)) * radius);
                gravityCenter.setPosition(newPosition);
                break;
            }
        }
    }

    // Update projectiles positions
    for(auto& projectile : mProjectiles)
    {
        switch(static_cast<AttackPattern::ID>(projectile->getPattern()))
        {
            case AttackPattern::Guided:
            {
                sf::Vector2f newVel = unitVector(dt.asSeconds() * getClosestTarget(projectile.get()) * 170.f + projectile->getVelocity()); // getVelocity and 170.f makes movement more parabolic
                newVel *= projectile->getMaxSpeed();
                projectile->setVelocity(newVel);
                break;
            }
            case AttackPattern::Spiral:
                break;
            case AttackPattern::Orbiting:
            {
                // Update center's position and apply angular displacement
                sf::Vector2f dir = projectile->getWorldPosition() /*- mPosition*/;
                float radius = vectorLength(dir);
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
                newPosition.y = mPosition.y + -(std::cos(toRadian(newAngle)) * radius);
                projectile->setPosition(newPosition);
                break;
            }
        }
    }

    for(auto& projectile : mProjectiles)
        projectile->update(dt, commandQueue);
}

void Attack::createProjectiles()
{
    const auto projectileNumber = attackData.at(mAttackID).projectiles.size();

    for(int i=0; i < projectileNumber; ++i)
        createProjectile(i);
}

void Attack::createProjectile(int num)
{
    Projectile::Type type = attackData.at(mAttackID).projectiles[num].type;
    float speed = attackData.at(mAttackID).projectiles[num].speed;
    std::unique_ptr<Projectile> projectile(new Projectile(type, mTextures, getWorld(), mShooterID, speed));

    sf::Vector2f direction;
    sf::Vector2f offset(attackData.at(mAttackID).projectiles[num].offset);
    projectile->setPosition(mPosition + offset);
    projectile->setPattern(attackData.at(mAttackID).projectiles[num].pattern);

    if(attackData.at(mAttackID).projectiles[num].isAimed && !mPossibleTargets.empty())
        direction = unitVector(getClosestTarget(projectile.get()));
    else
        direction = unitVector(attackData.at(mAttackID).projectiles[num].direction);

    sf::Vector2f velocity(direction * speed);
    projectile->setVelocity(velocity);

    mProjectiles.push_back(std::move(projectile));
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

Category::Type Attack::getCategory() const
{
    return Category::Attack;
}

sf::FloatRect Attack::getBoundingRect() const
{
    // Rect containing all projectiles
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

sf::Vector2f Attack::getClosestTarget(const sf::Transformable* object) const
/// Returns direction vector
{
    float smallestDistance = std::numeric_limits<float>::max();
    Aircraft* closestTarget = nullptr;

    for(const auto& target : mPossibleTargets)
    {
        if(target->getWorldPosition().y < object->getPosition().y) // Avoids projectile/gravityCenter turning back
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
