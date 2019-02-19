#include "Aircraft.hpp"
#include "Pickup.hpp"
#include "HealthBar.hpp"
#include "../Utility.hpp"
#include "../World.hpp"
#include "../DataTable.hpp"
#include "../AnimationNode.hpp"
#include <vector>
#include <memory>
namespace
{
    const std::vector<AircraftData> aircraftInfo = initializeAircraftData();
    const std::vector<AircraftTextureData> textureInfo = initializeAircraftTextureData();
}

Aircraft::Aircraft(int typeID, const TextureHolder& textures, const FontHolder& fonts, World& world, int id)
    : Entity(aircraftInfo[typeID].hitpoints, true, world),
      mTypeID(typeID),
      mSprite(textures.get(textureInfo[typeID].texture), textureInfo[typeID].textureRect),
      mFireRateLevel(2),
      mSpreadLevel(1),
      mMissileAmmo(2),
      mIdentifier(id),
      mIsFiring(false),
      mIsLaunchingMissile(false),
      mIsEnemy(mTypeID != 0),
      mShowExplosion(true),
      mTravelledDistance(0.f),
      mDirectionIndex(0),
      mTextures(textures),
      mAttackerID(0),
      mScore(0)
{
    centerOrigin(mSprite);

    // Create HealthBar for aircraft
    float offset = (mIsEnemy) ? -0.7f : 0.7f;
    std::unique_ptr<HealthBar> healthBar(new HealthBar(*this, aircraftInfo[mTypeID].hitpoints));
    healthBar->setPosition(0.f, mSprite.getLocalBounds().height * offset);
    attachChild(std::move(healthBar));

    mFireCommand.mCategories.push_back(Category::AirLayer);
    mFireCommand.mAction = [this, &textures](SceneNode& node, sf::Time)
    {
        shootBullets(node, textures);
    };

    mLaunchMissileCommand.mCategories.push_back(Category::AirLayer);
    mLaunchMissileCommand.mAction = [this, &textures](SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // Add more
    updateMovementPatterns(dt);
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
    if(!mIsEnemy)
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

void Aircraft::changeMissileAmmo(int amount)
{
    mMissileAmmo += amount;
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
    //if(aircraftInfo[mTypeID].fireInterval.asSeconds() > 0)
        //mIsFiring = true;
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
    return aircraftInfo[mTypeID].speed;
}

sf::FloatRect Aircraft::getLocalBounds() const
{
    return mSprite.getLocalBounds();
}

sf::FloatRect Aircraft::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Aircraft::updateRollAnimation(sf::Time dt)
{
    mLastRoll += dt;

    if(textureInfo[mTypeID].hasRollAnimation && mLastRoll.asSeconds() > 0.1f)
    {
        sf::IntRect currentRect = mSprite.getTextureRect();
        sf::IntRect defaultRect = textureInfo[mTypeID].textureRect;

        if(getVelocity().x > 0.f && currentRect.left < defaultRect.width * (textureInfo[mTypeID].spriteNumber-1))
        {
            currentRect.left += currentRect.width;
            mLastRoll = sf::Time::Zero;
        }
        else if(getVelocity().x < 0.f && currentRect.left > 0.f)
        {
            currentRect.left -= currentRect.width;
            mLastRoll = sf::Time::Zero;
        }
        else if(getVelocity().x == 0.f && currentRect != defaultRect)
        {
            (currentRect.left > defaultRect.left) ? currentRect.left -= currentRect.width
                                                       : currentRect.left += currentRect.width;
            mLastRoll = sf::Time::Zero;
        }

        mSprite.setTextureRect(currentRect);
    }
}

void Aircraft::shootBullets(SceneNode& layer, const TextureHolder& textures) const // TODO: Modify values
{
    Projectile::Type projectileType = (!mIsEnemy) ? Projectile::AlliedBullet : Projectile::EnemyBullet;

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
    /*std::unique_ptr<Projectile> projectile(new Projectile(type, textures, getWorld(), mIdentifier));

    float direction = (!mIsEnemy) ? -1.f : 1.f; // Decides if offsets will make projectile closer to top of window or closer to bottom
    sf::Vector2f offset(mSprite.getLocalBounds().width * xOffset,
                         mSprite.getLocalBounds().height * yOffset * direction);
    sf::Vector2f velocity(0, projectile->getMaxSpeed());

    projectile->setPosition(getWorldPosition() + offset);
    projectile->setVelocity(velocity * direction);
    layer.attachChild(std::move(projectile));*/
}

void Aircraft::launchProjectiles(sf::Time dt, CommandQueue& commands)
{
    if(mIsEnemy)
        fire();

    if(mIsFiring && mFireCooldown <= sf::Time::Zero)
    {
        commands.push(mFireCommand);
        getWorld().getSoundPlayer().play((mIsEnemy ? Sound::EnemyGun : Sound::AllyGun), getWorldPosition());
        //mFireCooldown += aircraftInfo[mTypeID].fireInterval / static_cast<float>(mFireRateLevel);
    }
    else if(mFireCooldown > sf::Time::Zero)
    {
        mFireCooldown -= dt;
    }
    mIsFiring = false;

    if(mIsLaunchingMissile)
    {
        commands.push(mLaunchMissileCommand);
        getWorld().getSoundPlayer().play(Sound::Missile, getWorldPosition());
        mIsLaunchingMissile = false;
    }
}

void Aircraft::updateMovementPatterns(sf::Time dt)
{
    const std::vector<AircraftData::Direction>& directions = aircraftInfo[mTypeID].directions;
    if(!directions.empty())
    {
        if(mTravelledDistance > directions[mDirectionIndex].distance)
        {
            mTravelledDistance = 0.f;
            mDirectionIndex = (mDirectionIndex+1) % directions.size();
        }

        float radians = toRadian(directions[mDirectionIndex].angle);
        float velx = getMaxSpeed() * std::sin(radians);
        float vely = getMaxSpeed() * std::cos(radians);

        setVelocity(velx, vely);
        mTravelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Aircraft::onCollision(Entity& entity)
{
    if(mIsEnemy)
    {
        switch(entity.getCategory())
        {
            case Category::PlayerAircraft:
                entity.damage(getHitpoints());
                destroy();
                mAttackerID = static_cast<Aircraft&>(entity).getIdentifier(); // Sets id of aircraft that will have score increased
                break;
        }
    }
}

void Aircraft::removeEntity()
{
    Entity::removeEntity();
    mShowExplosion = false; //
}

void Aircraft::increaseScore(int value)
{
    mScore += value;
}

int Aircraft::getScore() const
{
    return mScore;
}

void Aircraft::setAttackerID(int id)
{
    mAttackerID = id;
}

void Aircraft::createPickup() const
{
    if(mIsEnemy && (randomInt(1, 4) == 2)) // 25% chance for spawning pickup for enemies
    {
        auto type = static_cast<Pickup::Type>(randomInt(0, Pickup::TypeCount-1));
        std::unique_ptr<Pickup> pickup (new Pickup(type, mTextures, getWorld()));
        pickup->setPosition(getWorldPosition());
        getWorld().placeOnLayer(std::move(pickup), Category::AirLayer);
    }
}

void Aircraft::createExplosion() const
{
    if(mShowExplosion)
    {
        std::unique_ptr<AnimationNode> explosion(new AnimationNode(AnimationNode::Explosion, sf::seconds(0.06f), mTextures));
        explosion->setPosition(getWorldPosition());
        getWorld().placeOnLayer(std::move(explosion), Category::AirLayer);
        getWorld().getSoundPlayer().play(Sound::Explosion, getWorldPosition());
    }
}

void Aircraft::changeScore()
{
    if(mShowExplosion)
        increaseScoreRequest(100);
    else
        decreaseScoreRequest(50);
}

void Aircraft::increaseScoreRequest(int value) const
{
    Command increaseScoreCommand;
    increaseScoreCommand.mCategories.push_back(Category::PlayerAircraft);
    increaseScoreCommand.mAction = castFunctor<Aircraft>([this, value](Aircraft& aircraft, sf::Time dt)
    {
        if(aircraft.getIdentifier() == mAttackerID)
            aircraft.increaseScore(value);
    });

    getWorld().getCommandQueue().push(increaseScoreCommand);
}

void Aircraft::decreaseScoreRequest(int value) const
{
    Command decreaseScoreCommand;
    decreaseScoreCommand.mCategories.push_back(Category::PlayerAircraft);
    decreaseScoreCommand.mAction = castFunctor<Aircraft>([this, value](Aircraft& aircraft, sf::Time dt)
    {
        aircraft.increaseScore(-value);
    });

    getWorld().getCommandQueue().push(decreaseScoreCommand);
}

void Aircraft::onRemoval()
{
    createPickup();
    createExplosion();
    changeScore();
}
