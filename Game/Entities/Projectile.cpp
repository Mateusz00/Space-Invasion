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

Projectile::Projectile(Type type, const TextureHolder& textures, World& world, int shooterID, float speed)
    : Entity(1, true, world),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect),
      mShooterID(shooterID),
      mSpeed(speed)
{
    centerOrigin(mSprite);

    if(isGuided()) // Adds emitters for missiles
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke, getWorld().getParticleNode(), 30.f));
        smoke->setPosition(0.f, mSprite.getLocalBounds().height / 2.f);
        attachChild(std::move(smoke));

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant, getWorld().getParticleNode(), 30.f));
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

bool Projectile::isGuided() const
{
    return (mType == Type::Missile);
}

Category::Type Projectile::getCategory() const
{
    if(mType == EnemyBullet)
        return Category::EnemyProjectile;
    else
        return Category::AlliedProjectile;
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

AttackPattern::ID Projectile::getPattern() const
{
    return mPattern;
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
    if(getCategory() == Category::EnemyProjectile)
    {
        switch(entity.getCategory())
        {
            case Category::PlayerAircraft:
                entity.damage(table[mType].damage);
                destroy();
                break;
        }
    }
    else // Player's projectile
    {
        switch(entity.getCategory())
        {
            case Category::EnemyAircraft:
                entity.damage(table[mType].damage);
                destroy();
                static_cast<Aircraft&>(entity).setAttackerID(mShooterID); // Sets id of aircraft that will have score increased if enemy dies
                break;
        }
    }
}
