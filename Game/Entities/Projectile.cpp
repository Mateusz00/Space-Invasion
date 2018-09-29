#include "Projectile.hpp"
#include "../DataTable.hpp"
#include "../Utility.hpp"
#include <vector>
namespace
{
    const std::vector<ProjectileData> table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures, World& world)
    : Entity(1, true, world),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect)
{
    centerOrigin(mSprite);
}

float Projectile::getMaxSpeed() const
{
    return table[mType].speed;
}

int	Projectile::getDamage() const
{
    return table[mType].damage;
}

bool Projectile::isGuided() const
{
    return (mType == Type::Missile);
}

void Projectile::guideTowards(sf::Vector2f position)
{
    mTargetDirection = unitVector(position - getWorldPosition());
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

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if(isGuided())
    {
        sf::Vector2f newVelocity = unitVector(dt.asSeconds() * mTargetDirection * 170.f + getVelocity()); // getVelocity makes movement more parabolic
        newVelocity *= getMaxSpeed();
        setVelocity(newVelocity);

        float angle = std::atan2(newVelocity.y, newVelocity.x);
        setRotation(toDegree(angle) + 90.f);
    }
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
            case Category::EnemyAircraft:
            case Category::Pickup:
                break;

            default:
                entity.damage(table[mType].damage);
                break;
        }
    }
    else // Player's projectile
    {
        switch(entity.getCategory())
        {
            case Category::PlayerAircraft:
            case Category::Pickup:
                break;

            default:
                entity.damage(table[mType].damage);
                break;
        }
    }
}
