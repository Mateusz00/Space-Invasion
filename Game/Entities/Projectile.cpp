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

Projectile::Projectile(Type type, const TextureHolder& textures, World& world)
    : Entity(1, true, world),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect)
{
    centerOrigin(mSprite);

    if(isGuided()) // Adds emitters for missiles
	{
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke, getWorld().getParticleNode(), 20.f));
		smoke->setPosition(0.f, getLocalBounds().height / 2.f);
		attachChild(std::move(smoke));

		std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant, getWorld().getParticleNode(), 20.f));
		propellant->setPosition(0.f, getLocalBounds().height / 2.f);
		attachChild(std::move(propellant));
	}
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
                break;
        }
    }
}
