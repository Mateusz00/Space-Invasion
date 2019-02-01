#include "Attack.hpp"
#include "DataTable.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
namespace
{
    const std::unordered_map<int, AttackData> attackData = initializeAttackData();
}

Attack::Attack(int id, const TextureHolder& textures, sf::Vector2f pos)
    : mAttackID(id),
      mTextures(textures),
      mIsActive(false),
      mPosition(pos)
{
    mAttackCommand.mCategories.push_back(Category::AirLayer);
    mAttackCommand.mAction = [this](SceneNode& node, sf::Time)
    {
        for(int i=0; i < attackData[mAttackID].behaviors.size(); ++i)
            createProjectile(node, i);
    };
}

void Attack::pushProjectile(Projectile* projectile)
{
    mProjectiles.push_back(projectile);
}

void Attack::update(sf::Time dt, CommandQueue& commandQueue)
{
    if(!mIsActive)
    {
        setActive(true);
        commands.push(mAttackCommand);
    }

    //Complete later
}

void Attack::createProjectile(SceneNode& layer, int num)
{
    Projectile::Type type = attackData[mAttackID].types[num];
    std::unique_ptr<Projectile> projectile(new Projectile(type, mTextures, getWorld(), mIdentifier));

    sf::Vector2f offset(attackData[mAttackID].offsets[num]);
    sf::Vector2f direction(unitVector(attackData[mAttackID].directions[num]));
    sf::Vector2f velocity(direction * attackData[mAttackID].speeds[num]);

    projectile->setBehavior(attackData[mAttackID].behaviors[num]);
    projectile->setPosition(mPosition + offset);
    projectile->setVelocity(velocity);

    pushProjectile(projectile.get());
    layer.attachChild(std::move(projectile));
}

void Attack::setActive(bool isActive)
{
    mIsActive = isActive;
}
