#include "Attack.hpp"
#include "Attacks.hpp"
#include "../Category.hpp"
#include "../Entities/Aircraft.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
using Attacks::attackData;

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

    if(mProjectiles.empty())
        deactivate();

    // Update movement
    for(auto& projectile : mProjectiles)
    {
        switch(static_cast<Attack::Behavior>(projectile->getBehavior()))
        {
            case Behavior::Guided:
            {
                sf::Vector2f newVel = unitVector(dt.asSeconds() * getClosestTarget(projectile.get()) * 170.f + getVelocity()); // getVelocity and 170.f makes movement more parabolic
                newVel *= projectile->getMaxSpeed();
                projectile->setVelocity(newVel);
                break;
            }
            case Behavior::Spiral:
                break;
            case Behavior::Orbiting:
                break;
            case Behavior::Barrier:
                break;
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
    projectile->setBehavior(attackData.at(mAttackID).projectiles[num].behavior);

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

bool Attack::isBarrier() const
{
    const auto& projectilesArray = attackData.at(mAttackID).projectiles;

    for(const auto& projectile : projectilesArray)
    {
        if(projectile.behavior == Behavior::Barrier)
            return true;
    }

    return false;
}

void Attack::updatePosition(sf::Vector2f pos)
{
    mPosition = pos;
}

sf::Vector2f Attack::getClosestTarget(const Projectile* projectile) const
/// Returns direction vector
{
    float smallestDistance = std::numeric_limits<float>::max();
    Aircraft* closestTarget = nullptr;

    for(const auto& target : mPossibleTargets)
    {
        if(target->getWorldPosition().y < projectile->getWorldPosition().y) // Avoids projectile turning back
        {
            float targetDistance = vectorLength(projectile->getWorldPosition() - target->getWorldPosition());
            if(targetDistance < smallestDistance)
            {
                closestTarget = target;
                smallestDistance = targetDistance;
            }
        }
    }

    if(closestTarget)
        return unitVector(closestTarget->getWorldPosition() - projectile->getWorldPosition());
    else
        return sf::Vector2f();
}
