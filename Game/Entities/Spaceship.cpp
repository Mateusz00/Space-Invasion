#include "Spaceship.hpp"
#include "Pickup.hpp"
#include "Bar.hpp"
#include "../Utility.hpp"
#include "../World.hpp"
#include "../DataTable.hpp"
#include "../AnimationNode.hpp"
#include <vector>
#include <memory>
namespace
{
    const std::vector<SpaceshipData> spaceshipInfo = initializeSpaceshipData();
    const std::vector<SpaceshipTextureData> textureInfo = initializeSpaceshipTextureData();
    constexpr float FUEL_MAX = 100.f;
    constexpr float FUEL_MIN = 25.f;
    constexpr float FUEL_BURN_RATE = 4.4f;
    constexpr float FUEL_REFILL_RATE = 0.3f;
}

Spaceship::Spaceship(int typeID, const TextureHolder& textures, const FontHolder& fonts,
                    World& world, const std::vector<Spaceship*>& targets, int id)
    : Entity(spaceshipInfo[typeID].hitpoints, true, world),
      mTypeID(typeID),
      mSprite(textures.get(textureInfo[typeID].texture), textureInfo[typeID].textureRect),
      mFireRateLevel(3),
      mSpreadLevel(1),
      mMissileAmmo(2),
      mIdentifier(id),
      mIsEnemy(typeID != 0),
      mShowExplosion(true),
      mTravelledDistance(0.f),
      mDirectionIndex(0),
      mTextures(textures),
      mAttackerID(0),
      mScore(0),
      mAttackManager(textures, world, id, !mIsEnemy, targets),
      mBoosted(false),
      mBoostCooldown(false),
      mBoostFuel(FUEL_MAX),
      mBoostFuelBar(nullptr)
{
    centerOrigin(mSprite);

    // Create HealthBar for spaceship
    float offset = (mIsEnemy) ? -0.7f : 0.7f;
    sf::Vector2f barSize(getLocalBounds().width * 0.7f, 4.f);

    std::unique_ptr<Bar> healthBar(new Bar(getHitpoints(), spaceshipInfo[mTypeID].hitpoints, barSize));
    healthBar->setPosition(0.f, mSprite.getLocalBounds().height * offset);
    healthBar->setColorRange(sf::Color(33, 196, 1), sf::Color(206, 12, 12));
    mHealthBar = healthBar.get();
    attachChild(std::move(healthBar));

    // Create boost fuel bar for players
    if(!mIsEnemy)
    {
        std::unique_ptr<Bar> fuelBar(new Bar(mBoostFuel, FUEL_MAX, sf::Vector2f(barSize.x, barSize.y - 1.f)));
        fuelBar->setPosition(0.f, mSprite.getLocalBounds().height * offset + barSize.y);
        fuelBar->setColorRange(sf::Color(224, 188, 6), sf::Color(224, 188, 6));
        mBoostFuelBar = fuelBar.get();
        attachChild(std::move(fuelBar));
    }

    // Add attacks to manager
    const auto& attacks = spaceshipInfo[typeID].attacks;
    for(const auto& attackPair : attacks)
        mAttackManager.pushAttack(attackPair.first, attackPair.second);
}

void Spaceship::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateMovementPatterns(dt);
    updateRollAnimation(dt);
    Entity::updateCurrent(dt, commands);

    mAttackManager.updatePosition(getWorldPosition());
    mAttackManager.update(dt, commands);

    if(mBoostFuelBar)
    {
        updateBoostFuel();
        mBoostFuelBar->updateValue(mBoostFuel);
    }
    mHealthBar->updateValue(getHitpoints());
}

void Spaceship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

Category::Type Spaceship::getCategory() const
{
    if(!mIsEnemy)
        return Category::PlayerSpaceship;
    else
        return Category::EnemySpaceship;
}

void Spaceship::increaseFireRate()
{
    if(mFireRateLevel < 6)
        ++mFireRateLevel;
}

void Spaceship::increaseSpread()
{
    if(mSpreadLevel < 3)
        ++mSpreadLevel;
}

int Spaceship::getMissileAmmo() const
{
    return mMissileAmmo;
}

void Spaceship::setMissileAmmo(int missiles)
{
    mMissileAmmo = missiles;
}

void Spaceship::changeMissileAmmo(int amount)
{
    mMissileAmmo += amount;
}

int Spaceship::getIdentifier() const
{
    return mIdentifier;
}

void Spaceship::setIdentifier(int id)
{
    mIdentifier = id;
}

void Spaceship::fire()
{
    if(mAttackManager.tryAttack(mSpreadLevel+1, getWorld().getCommandQueue()))
        mAttackManager.forceCooldown(mAttackManager.getCooldown() / static_cast<float>(mFireRateLevel));
}

void Spaceship::boostSpeed()
{
    if(!mBoostCooldown && mBoostFuel >= FUEL_BURN_RATE)
        mBoosted = true;
}

void Spaceship::trySpeedBoost()
{
    if(mBoosted)
        setVelocity(getVelocity() * 2.f);
}

void Spaceship::launchMissile()
{
    if(mMissileAmmo > 0)
    {
        mAttackManager.forceAttack(0, getWorld().getCommandQueue(), false);
        --mMissileAmmo;
    }
}

float Spaceship::getMaxSpeed() const
{
    return spaceshipInfo[mTypeID].speed;
}

sf::FloatRect Spaceship::getLocalBounds() const
{
    return mSprite.getLocalBounds();
}

sf::FloatRect Spaceship::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Spaceship::updateRollAnimation(sf::Time dt)
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

void Spaceship::updateMovementPatterns(sf::Time dt)
{
    const std::vector<SpaceshipData::Direction>& directions = spaceshipInfo[mTypeID].directions;
    if(!directions.empty())
    {
        if(mTravelledDistance > directions[mDirectionIndex].distance)
        {
            mTravelledDistance = 0.f;
            mDirectionIndex = (mDirectionIndex+1) % directions.size();
        }

        float radians = toRadian(directions[mDirectionIndex].angle);
        float speedPercentage = directions[mDirectionIndex].speedPercentage/100.f;
        float velx = getMaxSpeed() * std::sin(radians) * speedPercentage;
        float vely = getMaxSpeed() * std::cos(radians) * speedPercentage;

        setVelocity(velx, vely);
        mTravelledDistance += getMaxSpeed() * dt.asSeconds() * speedPercentage;
    }
}

void Spaceship::onCollision(Entity& entity)
{
    if(mIsEnemy)
    {
        switch(entity.getCategory())
        {
            case Category::PlayerSpaceship:
                entity.damage(getHitpoints());
                destroy();
                mAttackerID = static_cast<Spaceship&>(entity).getIdentifier(); // Sets id of spaceship that will have score increased
                break;
        }
    }
}

void Spaceship::removeEntity()
{
    Entity::removeEntity();
    mShowExplosion = false;
}

void Spaceship::increaseScore(int value)
{
    mScore += value;
}

int Spaceship::getScore() const
{
    return mScore;
}

void Spaceship::setAttackerID(int id)
{
    mAttackerID = id;
}

void Spaceship::createPickup() const
{
    if(mIsEnemy && (randomInt(1, 4) == 2)) // 25% chance for spawning pickup for enemies
    {
        auto type = static_cast<Pickup::Type>(randomInt(0, Pickup::TypeCount-1));
        std::unique_ptr<Pickup> pickup (new Pickup(type, mTextures, getWorld()));
        pickup->setPosition(getWorldPosition());
        getWorld().placeOnLayer(std::move(pickup), Category::AirLayer);
    }
}

void Spaceship::createExplosion() const
{
    if(mShowExplosion)
    {
        std::unique_ptr<AnimationNode> explosion(new AnimationNode(AnimationNode::Explosion, sf::seconds(0.06f), mTextures));
        explosion->setPosition(getWorldPosition());
        getWorld().placeOnLayer(std::move(explosion), Category::AirLayer);
        getWorld().getSoundPlayer().play(Sound::Explosion, getWorldPosition());
    }
}

void Spaceship::changeScore()
{
    if(mShowExplosion)
        increaseScoreRequest(100);
    else
        decreaseScoreRequest(50);
}

void Spaceship::increaseScoreRequest(int value) const
{
    Command increaseScoreCommand;
    increaseScoreCommand.mCategories.push_back(Category::PlayerSpaceship);
    increaseScoreCommand.mAction = castFunctor<Spaceship>([this, value](Spaceship& spaceship, sf::Time dt)
    {
        if(spaceship.getIdentifier() == mAttackerID)
            spaceship.increaseScore(value);
    });

    getWorld().getCommandQueue().push(increaseScoreCommand);
}

void Spaceship::decreaseScoreRequest(int value) const
{
    Command decreaseScoreCommand;
    decreaseScoreCommand.mCategories.push_back(Category::PlayerSpaceship);
    decreaseScoreCommand.mAction = castFunctor<Spaceship>([this, value](Spaceship& spaceship, sf::Time dt)
    {
        spaceship.increaseScore(-value);
    });

    getWorld().getCommandQueue().push(decreaseScoreCommand);
}

void Spaceship::onRemoval()
{
    createPickup();
    createExplosion();
    changeScore();
    mAttackManager.onRemoval();
}

void Spaceship::updateBoostFuel()
{
    if(mBoosted)
    {
        mBoostFuel = std::max(mBoostFuel - FUEL_BURN_RATE, 0.f);
        mBoosted = false;
    }
    else
    {
        mBoostFuel = std::min(mBoostFuel + FUEL_REFILL_RATE, FUEL_MAX);

        if(mBoostFuel < FUEL_MIN)
            mBoostCooldown = true;
        else
            mBoostCooldown = false;
    }
}
