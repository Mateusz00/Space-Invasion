#include "Attack.hpp"
#include "Attacks.hpp"
#include "Category.hpp"
#include "DataTable.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
using namespace Attacks;

Attack::Attack(int id, const TextureHolder& textures, sf::Vector2f pos, World& world, int shooterID)
    : Entity(1, false, world),
      mAttackID(id),
      mTextures(textures),
      mIsActive(true),
      mIsReadyToDelete(false),
      mPosition(pos),
      mShooterID(shooterID)
{
    const auto projectileNumber = attackData.at(mAttackID).types.size();

    mAttackCommand.mCategories.push_back(Category::AirLayer);
    mAttackCommand.mAction = [this, projectileNumber](SceneNode& node, sf::Time)
    {
        for(int i=0; i < projectileNumber; ++i)
            createProjectile(node, i);
    };
}

Attack::~Attack()
{
    // Notify AttackManager about removal of Attack
}

void Attack::update(sf::Time dt, CommandQueue& commandQueue)
{
    // Delete all projectiles that are marked for removal
    //Complete later
}

void Attack::createProjectile(SceneNode& layer, int num)
{
    /*Projectile::Type type = attackData.at(mAttackID).types[num];
    mProjectiles.emplace_back(type, mTextures, getWorld(), mShooterID);
    auto projectile = mProjectiles.back();

    sf::Vector2f offset(attackData.at(mAttackID).offsets[num]);
    sf::Vector2f direction(unitVector(attackData.at(mAttackID).directions[num]));
    sf::Vector2f velocity(direction * attackData.at(mAttackID).speeds[num]);

    projectile.setBehavior(attackData.at(mAttackID).behavior[num]);
    projectile.setPosition(mPosition + offset);
    projectile.setVelocity(velocity);*/
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

void Attack::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
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
    const auto& behaviorArray = attackData.at(mAttackID).behavior;

    for(const auto& behavior : behaviorArray)
    {
        if(behavior == Behavior::Barrier)
            return true;
    }

    return false;
}

void Attack::updatePosition(sf::Vector2f pos)
{
    mPosition = pos;
}
