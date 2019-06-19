#include "Spaceship.hpp"
#include "Pickup.hpp"
#include "Bar.hpp"
#include "../Utility.hpp"
#include "../SpriteNode.hpp"
#include "../World.hpp"
#include "../DataTable.hpp"
#include "../AnimationNode.hpp"
#include "../CollisionResponseMap.hpp"
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
bool Spaceship::mHasInitializedResponses = false;

Spaceship::Spaceship(int typeID, const TextureHolder& textures, const FontHolder& fonts,
                    ObjectContext context, const std::vector<Spaceship*>& targets, int id)
    : Entity(spaceshipInfo[typeID].hitpoints, true, context),
      mTypeID(typeID),
      mFireRateLevel(6),
      mSpreadLevel(0),
      mMissileAmmo(2),
      mPlayerID(id),
      mIsEnemy(typeID != 0),
      mShowExplosion(true),
      mTravelledDistance(0.f),
      mDirectionIndex(0),
      mTextures(textures),
      mAttackerID(0),
      mScore(0),
      mAttackManager(textures, context, id, !mIsEnemy, &targets),
      mBoosted(false),
      mBoostCooldown(false),
      mBoostFuel(FUEL_MAX),
      mBoostFuelBar(nullptr),
      mEnemyType(EnemyType::Normal)
{
    if(mIsEnemy)
        addCategories(Category::EnemySpaceship);
    else
        addCategories(Category::PlayerSpaceship);

    // Initialize sprite
    int textureID = spaceshipInfo[typeID].textureID;
    int animationID = spaceshipInfo[typeID].animationID;
    if(textureID >= 0)
    {
        mSprite.reset(new SpriteNode(textures.get(textureInfo[textureID].texture), textureInfo[textureID].textureRect));
    }
    else
    {
        auto id = static_cast<Animation::ID>(animationID);
        std::unique_ptr<AnimationNode> sprite(new AnimationNode(id, sf::seconds(0.25f), mTextures));
        sprite->setAnimationType(AnimationNode::AnimationType::ForwardAndBackward);
        sprite->setRepeating(true);
        mSprite = std::move(sprite);
    }

    mSprite->center();
    if(mIsEnemy)
        mSprite->rotate(180.f);

    // Create HealthBar for spaceship
    float offset = (mIsEnemy) ? -0.7f : 0.7f;
    sf::Vector2f barSize(getLocalBounds().width * 0.7f, 4.f);

    std::unique_ptr<Bar> healthBar(new Bar(getHitpoints(), spaceshipInfo[mTypeID].hitpoints, barSize));
    healthBar->setPosition(0.f, getLocalBounds().height * offset);
    healthBar->setColorRange(sf::Color(33, 196, 1), sf::Color(206, 12, 12));
    mHealthBar = healthBar.get();
    attachChild(std::move(healthBar));

    // Create boost fuel bar for players
    if(!mIsEnemy)
    {
        std::unique_ptr<Bar> fuelBar(new Bar(mBoostFuel, FUEL_MAX, sf::Vector2f(barSize.x, barSize.y - 1.f)));
        fuelBar->setPosition(0.f, getLocalBounds().height * offset + barSize.y);
        fuelBar->setColorRange(sf::Color(224, 188, 6), sf::Color(224, 188, 6));
        mBoostFuelBar = fuelBar.get();
        attachChild(std::move(fuelBar));
    }

    // Add attacks to manager
    const auto& attacks = spaceshipInfo[typeID].attacks;
    for(const auto& attackPair : attacks)
        mAttackManager.pushAttack(attackPair.first, attackPair.second);

    if(!mHasInitializedResponses)
        initializeCollisionResponses();
}

void Spaceship::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    mSprite->update(dt, commands);
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
    target.draw(*mSprite, states);
}

void Spaceship::increaseFireRate()
{
    if(mFireRateLevel < 9)
        ++mFireRateLevel;
}

void Spaceship::increaseSpread()
{
    if(mSpreadLevel < 2)
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

int Spaceship::getPlayerID() const
{
    return mPlayerID;
}

void Spaceship::setIdentifier(int id)
{
    mPlayerID = id;
}

void Spaceship::fire()
{
    if(mAttackManager.tryAttack(mSpreadLevel+200, *(getObjectContext().commandQueue)))
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
        mAttackManager.forceAttack(0, *(getObjectContext().commandQueue), false);
        --mMissileAmmo;
    }
}

float Spaceship::getMaxSpeed() const
{
    return spaceshipInfo[mTypeID].speed;
}

sf::FloatRect Spaceship::getLocalBounds() const
{
    return mSprite->getBoundingRect();
}

sf::FloatRect Spaceship::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite->getBoundingRect());
}

void Spaceship::updateRollAnimation(sf::Time dt)
{
    int textureID = spaceshipInfo[mTypeID].textureID;
    mLastRoll += dt;

    if(!mIsEnemy && mLastRoll.asSeconds() > 0.15f && textureID >= 0)
    {
        sf::IntRect currentRect = mSprite->getTextureRect();
        sf::IntRect defaultRect = textureInfo[textureID].textureRect;

        if(getVelocity().x > 0.f && currentRect.left < defaultRect.width * (textureInfo[textureID].spriteNumber-1))
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

        mSprite->setTextureRect(currentRect);
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

void Spaceship::initializeCollisionResponses()
{
    mHasInitializedResponses = true;

    auto response1 = castResponse<Spaceship, Spaceship>([](Spaceship& player, Spaceship& enemy)
    {
        int entityHitpoints = player.getHitpoints();
        player.damage(enemy.getHitpoints());
        enemy.damage(entityHitpoints);
        enemy.setAttackerID(player.getEntityID()); // Sets id of spaceship that will have score increased
    });

    Category::Type cat1 = static_cast<Category::Type>(Category::PlayerSpaceship | Category::Collidable);
    Category::Type cat2 = static_cast<Category::Type>(Category::EnemySpaceship  | Category::Collidable);

    CollisionResponseMap::addResponse(cat1, cat2, response1);
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

void Spaceship::setEnemyType(EnemyType type)
{
    mEnemyType = type;
}

void Spaceship::createPickup() const
{
    if(mIsEnemy && (randomInt(1, 4) == 2)) // 25% chance for spawning pickup for enemies
    {
        const TextureHolder& t = mTextures;
        Command createPickupCommand;

        createPickupCommand.mCategories = Category::AirLayer;
        createPickupCommand.mAction = [pos = getWorldPosition(), context = getObjectContext(), &t](SceneNode& layer, sf::Time)
        {
            auto type = static_cast<Pickup::Type>(randomInt(0, Pickup::TypeCount-1));
            std::unique_ptr<Pickup> pickup(new Pickup(type, t, context));
            pickup->setPosition(pos);
            layer.attachChild(std::move(pickup));
        };

        getObjectContext().commandQueue->push(createPickupCommand);
    }
}

void Spaceship::createExplosion() const
{
    if(mShowExplosion)
    {
        sendExplosion(getWorldPosition());
        getObjectContext().soundPlayer->play(Sound::Explosion, getWorldPosition());
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
    increaseScoreCommand.mCategories = Category::PlayerSpaceship;
    increaseScoreCommand.mAction = castFunctor<Spaceship>([this, value](Spaceship& spaceship, sf::Time dt)
    {
        if(spaceship.getPlayerID() == mAttackerID)
            spaceship.increaseScore(value);
    });

    getObjectContext().commandQueue->push(increaseScoreCommand);
}

void Spaceship::decreaseScoreRequest(int value) const
{
    Command decreaseScoreCommand;
    decreaseScoreCommand.mCategories = Category::PlayerSpaceship;
    decreaseScoreCommand.mAction = castFunctor<Spaceship>([this, value](Spaceship& spaceship, sf::Time dt)
    {
        spaceship.increaseScore(-value);
    });

    getObjectContext().commandQueue->push(decreaseScoreCommand);
}

void Spaceship::onRemoval()
{
    switch(mEnemyType)
    {
        case EnemyType::Boss:
        {
            sf::Vector2f currentPos = getWorldPosition();
            sendExplosion(currentPos + sf::Vector2f(100.f, 40.f));
            sendExplosion(currentPos + sf::Vector2f(-20.f, -30.f));
            break;
        }
    }

    createPickup();
    createExplosion();
    changeScore();
    mAttackManager.onRemoval();
}

void Spaceship::sendExplosion(sf::Vector2f pos) const
{
    const TextureHolder& t = mTextures;
    Command explosionCommand;

    explosionCommand.mCategories = Category::AirLayer;
    explosionCommand.mAction = [pos, &t](SceneNode& layer, sf::Time)
    {
        std::unique_ptr<AnimationNode> node(new AnimationNode(Animation::Explosion, sf::seconds(0.06f), t, pos));
        layer.attachChild(std::move(node));
    };

    getObjectContext().commandQueue->push(explosionCommand);
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
