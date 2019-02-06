#include "Attack.hpp"
#include "Category.hpp"
#include "DataTable.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
namespace
{
    const std::unordered_map<int, AttackData> attackData = initializeAttackData();
}

Attack::Attack(int id, const TextureHolder& textures, sf::Vector2f pos, World& world, AttackManager& manager)
    : Entity(1, false, world),
      mAttackID(id),
      mTextures(textures),
      mIsActive(false),
      mPosition(pos),
      mAttackManager(manager)
{
    mAttackCommand.mCategories.push_back(Category::AirLayer);
    mAttackCommand.mAction = [this](SceneNode& node, sf::Time)
    {
        for(int i=0; i < attackData[mAttackID].behaviors.size(); ++i)
            createProjectile(node, i);
    };
}

Attack::~Attack()
{
    // Notify AttackManager about removal of Attack
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

Category::Type Attack::getCategory()
{
    return Category::Attack;
}

sf::FloatRect Attack::getBoundingRect()
{
    // Rect containing all projectiles
}

void Attack::drawCurrent(sf::RenderTarget&, sf::RenderStates)
{
    // Draw projectiles
}

void Attack::updateCurrent(sf::Time, CommandQueue&)
{
    // Update movement
}

bool Attack::isMarkedForRemoval()
{
    // True when all projectiles are markedForRemoval or mProjectiles is empty and attack is active
}
