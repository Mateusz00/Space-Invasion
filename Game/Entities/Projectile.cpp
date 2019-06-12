#include "Projectile.hpp"
#include "../DataTable.hpp"
#include "../Utility.hpp"
#include "../World.hpp"
#include "../ParticleSystem/EmitterNode.hpp"
#include <vector>
#include <memory>
namespace
{
    const std::vector<ProjectileData> table = initializeProjectileData();
}

Projectile::Projectile(Projectiles::ID type, const TextureHolder& textures,
                        ObjectContext context, int shooterID, float speed, bool isEnemy)
    : Entity(table[type].hitpoints, true, context),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect),
      mShooterID(shooterID),
      mSpeed(speed),
      mIsEnemy(isEnemy)
{
    if(isEnemy)
        addCategories(Category::EnemyProjectile);
    else
        addCategories(Category::AlliedProjectile);

    centerOrigin(mSprite);

    if(mType == Projectiles::Missile) // Adds emitters for missiles
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke, getObjectContext().particleNode, 30.f));
        smoke->setPosition(0.f, mSprite.getLocalBounds().height / 2.f);
        attachChild(std::move(smoke));

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant, getObjectContext().particleNode, 30.f));
        propellant->setPosition(0.f, mSprite.getLocalBounds().height / 2.f);
        attachChild(std::move(propellant));
    }
}

float Projectile::getMaxSpeed() const
{
    return mSpeed;
}

int Projectile::getDamage() const
{
    return table[mType].damage;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

int Projectile::getShootersID() const
{
    return mShooterID;
}

void Projectile::setPattern(AttackPattern::ID id)
{
    mPattern = id;
}

void Projectile::setPatternData(AttackPattern::PatternData data)
{
    mPatternData = data;
}

AttackPattern::ID Projectile::getPattern() const
{
    return mPattern;
}

AttackPattern::PatternData Projectile::getPatternData() const
{
    return mPatternData;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    float angle = std::atan2(getVelocity().y, getVelocity().x);
    setRotation(toDegree(angle) + 90.f);

    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Projectile::onCollision(Entity& entity)
{
    auto categories = entity.getCategories();

    if(mIsEnemy)
    {
        if(categories & Category::PlayerSpaceship)
        {
            entity.damage(table[mType].damage);
            damage(table[mType].damage);
        }
    }
    else // Player's projectile
    {
        if(categories & Category::EnemySpaceship)
        {
            entity.damage(table[mType].damage);
            damage(table[mType].damage);
            static_cast<Spaceship&>(entity).setAttackerID(mShooterID); // Sets id of spaceship that will have score increased if enemy dies
        }
    }
}

void Projectile::setDirection(sf::Vector2f dir)
{
    mDirection = dir;
}

sf::Vector2f Projectile::getDirection() const
{
    return mDirection;
}

void Projectile::setStartPos(sf::Vector2f pos)
{
    mStartPos = pos;
}

sf::Vector2f Projectile::getStartPos() const
{
    return mStartPos;
}

///@return seconds that elapsed since creation of this projectile
float Projectile::activeTime() const
{
    return mTimeActive.asSeconds();
}

void Projectile::updateTime(sf::Time dt)
{
    mTimeActive += dt;
}
