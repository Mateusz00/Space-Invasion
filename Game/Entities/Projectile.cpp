#include "Projectile.hpp"
#include "../DataTable.hpp"
#include "../Utility.hpp"
#include <vector>
namespace
{
    const std::vector<ProjectileData> table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
    : Entity(1),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect)
{
    centerOrigin(mSprite);

    float direction = (type == AlliedBullet) -1.f : 1.f;
    sf::Vector2f velocity(0.f, getMaxSpeed() * direction);
    setVelocity(velocity);
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
    mTargetDirection = oneUnitVector(position - getWorldPosition());
}

Category::Type Projectile::getCategory() const
{
	if(mType == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if(isGuided())
    {
        // Add homing missile function
    }
    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
