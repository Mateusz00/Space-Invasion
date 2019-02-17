#include "Attack.hpp"
#include "Attacks.hpp"
#include "../Category.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
using Attacks::attackData;

Attack::Attack(int id, const TextureHolder& textures, sf::Vector2f pos, World& world, int shooterID)
    : Entity(1, false, world),
      mAttackID(id),
      mTextures(textures),
      mIsActive(true),
      mIsReadyToDelete(false),
      mPosition(pos),
      mShooterID(shooterID)
{
    const auto projectileNumber = attackData.at(mAttackID).projectiles.size();
    for(int i=0; i < projectileNumber; ++i)
        createProjectile(i);
}

Attack::~Attack()
{
    // Notify AttackManager about removal of Attack
}

void Attack::update(sf::Time dt, CommandQueue& commandQueue)
{
    auto iter = std::remove_if(mProjectiles.begin(), mProjectiles.end(), std::mem_fn(&Projectile::isMarkedForRemoval));
    mProjectiles.erase(iter, mProjectiles.end());

    //Complete later
}

void Attack::createProjectile(int num)
{
    Projectile::Type type = attackData.at(mAttackID).projectiles[num].type;
    std::unique_ptr<Projectile> projectile(new Projectile(type, mTextures, getWorld(), mShooterID));

    sf::Vector2f offset(attackData.at(mAttackID).projectiles[num].offset);
    sf::Vector2f direction(unitVector(attackData.at(mAttackID).projectiles[num].direction));
    sf::Vector2f velocity(direction * attackData.at(mAttackID).projectiles[num].speed);

    projectile->setBehavior(attackData.at(mAttackID).projectiles[num].behavior);
    projectile->setPosition(mPosition + offset);
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
    mProjectiles.clear();
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
    // Draw projectiles
}

void Attack::updateCurrent(sf::Time, CommandQueue&)
{
    // Update movement
    // Deactivate if no projectiles
}

void Attack::removeEntity()
{
    deactivate();
}

bool Attack::isMarkedForRemoval() const
{
    // True when all projectiles are markedForRemoval or mProjectiles is empty and attack is active and isReadyToDelete
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
