#include "Aircraft.hpp"
#include "HealthBar.hpp"
#include "../Utility.hpp"
#include "../DataTable.hpp"
#include <vector>
#include <memory>
namespace
{
    const std::vector<AircraftData> table = initializeAircraftData();
}


Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(table[type].hitpoints),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect),
      mFireRateLevel(2),
      mSpreadLevel(1),
      mMissileAmmo(2),
      mIdentifier(0),
      mIsFiring(false),
      mIsLaunchingMissile(false)
{
    centerOrigin(mSprite);

    // Create HealthBar for aircraft
    sf::FloatRect spriteDimensions = mSprite.getLocalBounds();
    std::unique_ptr<HealthBar> healthBar(new HealthBar(*this, table[mType].hitpoints));
    healthBar->setPosition(0.f, spriteDimensions.height*0.7f);
    attachChild(std::move(healthBar));

    mFireCommand.mCategories.push_back(Category::AirLayer);
    mFireCommand.mAction = [this, &textures](SceneNode& node, sf::Time dt)
    {
        shootBullets(node, textures);
    };

    mLaunchMissileCommand.mCategories.push_back(Category::AirLayer);
    mLaunchMissileCommand.mAction = [this, &textures](SceneNode& node, sf::Time dt)
    {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // Add more
    updateRollAnimation(dt);
    launchProjectiles(dt, commands);
    Entity::updateCurrent(dt, commands);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

Category::Type Aircraft::getCategory() const
{
    if(mType == Ally)
        return Category::PlayerAircraft;
    else
        return Category::EnemyAircraft;
}

void Aircraft::increaseFireRate()
{
    if(mFireRateLevel < 8)
        ++mFireRateLevel;
}

void Aircraft::increaseSpread()
{
    if(mSpreadLevel < 5)
        ++mSpreadLevel;
}

int Aircraft::getMissileAmmo() const
{
    return mMissileAmmo;
}

void Aircraft::setMissileAmmo(int missiles)
{
    mMissileAmmo = missiles;
}

int  Aircraft::getIdentifier() const
{
    return mIdentifier;
}

void Aircraft::setIdentifier(int id)
{
    mIdentifier = id;
}

void Aircraft::fire()
{
    if(table[mType].fireInterval.asSeconds() > 0)
        mIsFiring = true;
}

void Aircraft::launchMissile()
{
    if(mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

float Aircraft::getMaxSpeed() const
{
    return table[mType].speed;
}

sf::FloatRect Aircraft::getLocalBounds() const
{
    return mSprite.getLocalBounds();
}

void Aircraft::updateRollAnimation(sf::Time dt)
{
    static sf::Time lastRoll;
    lastRoll += dt;

    if(table[mType].hasRollAnimation && lastRoll.asSeconds() > 0.09f)
    {
        sf::IntRect currentRect = mSprite.getTextureRect();
        sf::IntRect deafultRect = table[mType].textureRect;

        if(getVelocity().x > 0.f && currentRect.left < currentRect.width * (table[mType].spriteNumber-1))
        {
            currentRect.left += currentRect.width;
            lastRoll = sf::Time::Zero;
        }
        else if(getVelocity().x < 0.f && currentRect.left > 0.f)
        {
            currentRect.left -= currentRect.width;
            lastRoll = sf::Time::Zero;
        }
        else if(getVelocity().x == 0.f && currentRect != deafultRect)
        {
            (currentRect.left > deafultRect.left) ? currentRect.left -= currentRect.width
                                                       : currentRect.left += currentRect.width;
            lastRoll = sf::Time::Zero;
        }

        mSprite.setTextureRect(currentRect);
    }
}

void Aircraft::shootBullets(SceneNode& layer, const TextureHolder& textures) const // TODO: Modify values
{
    Projectile::Type projectileType = (mType == Ally) ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch(mSpreadLevel)
	{
		case 1:
			createProjectile(layer, projectileType,  0.0f,  0.5f,  textures);
			break;

		case 2:
			createProjectile(layer, projectileType, -0.2f,  0.5f,  textures);
			createProjectile(layer, projectileType,  0.2f,  0.5f,  textures);
			break;

		case 3:
			createProjectile(layer, projectileType, -0.25f, 0.25f, textures);
			createProjectile(layer, projectileType,  0.0f,  0.5f,  textures);
			createProjectile(layer, projectileType,  0.25f, 0.25f, textures);
			break;

		case 4:
			createProjectile(layer, projectileType, -0.4f,  0.25f, textures);
			createProjectile(layer, projectileType, -0.15f, 0.5f,  textures);
			createProjectile(layer, projectileType,  0.15f, 0.5f,  textures);
			createProjectile(layer, projectileType,  0.4f,  0.25f, textures);
			break;

		case 5:
			createProjectile(layer, projectileType, -0.48f,  0.25f, textures);
			createProjectile(layer, projectileType, -0.2f,  0.5f,  textures);
			createProjectile(layer, projectileType,  0.0f,  0.75f, textures);
			createProjectile(layer, projectileType,  0.2f,  0.5f,  textures);
			createProjectile(layer, projectileType,  0.48f,  0.25f, textures);
			break;
	}
}

void Aircraft::createProjectile(SceneNode& layer, Projectile::Type type, float xOffset,
                                 float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

    float direction = (type == Projectile::AlliedBullet) ? -1.f : 1.f; // Decides if offsets will make projectile closer to top of window or closer to bottom
    sf::Vector2f offset(mSprite.getLocalBounds().width * xOffset,
                         mSprite.getLocalBounds().height * yOffset * direction);

    projectile->setPosition(getWorldPosition() + offset);
    layer.attachChild(std::move(projectile));
}

void Aircraft::launchProjectiles(sf::Time dt, CommandQueue& commands)
{
    if(mType != Ally)
        fire();

    if(mIsFiring && mFireCooldown <= sf::Time::Zero)
    {
        commands.push(mFireCommand);
        mFireCooldown += table[mType].fireInterval / static_cast<float>(mFireRateLevel);
    }
    else if(mFireCooldown > sf::Time::Zero)
    {
        mFireCooldown -= dt;
    }
    mIsFiring = false;

    if(mIsLaunchingMissile)
	{
		commands.push(mLaunchMissileCommand);
		mIsLaunchingMissile = false;
	}
}
