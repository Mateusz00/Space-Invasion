#include "AttackManager.hpp"
#include "Attack.hpp"
#include "../Entities/Aircraft.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
#include "Attacks.hpp"

AttackManager::AttackManager(const TextureHolder& textures, World& world, int shooterID, bool isAllied)
    : mTextures(textures),
      mWorld(world),
      mShooterID(shooterID),
      mIsAllied(isAllied)
{
    initializeCommands();
}

void AttackManager::pushAttack(int id, int probability)
{
    mAttacks.emplace_back(id, probability);
}

void AttackManager::useAttack(int id, CommandQueue& commands)
{
    if(id == -1)
        return;

    Command launchAttack;
    launchAttack.mCategories.push_back(Category::AirLayer);
    launchAttack.mAction = [this, id](SceneNode& layer, sf::Time)
    {
        std::unique_ptr<Attack> attack(new Attack(id, mTextures, mPosition, mWorld, mShooterID, mPossibleTargets, mIsAllied));
        mCurrentAttacks.emplace_back(attack.get());
        layer.attachChild(std::move(attack));
    };

    commands.push(launchAttack);
    mCooldown += Attacks::attackData.at(id).cooldown;
}

void AttackManager::forceAttack(int id, CommandQueue& commands)
{
    useAttack(id, commands);
}

sf::Time AttackManager::getCooldown() const
{
    return mCooldown;
}

void AttackManager::forceCooldown(sf::Time cooldown)
{
    mCooldown = cooldown;
}

bool AttackManager::tryAttack(int id, CommandQueue& commands) // For players attacks
{
    if(mCooldown <= sf::Time::Zero)
    {
        useAttack(id, commands);
        return true;
    }

    return false;
}

void AttackManager::update(sf::Time dt, CommandQueue& commandQueue)
{
    mCooldown = std::max(mCooldown - dt, sf::Time::Zero);

    if(mCooldown <= sf::Time::Zero && !mIsAllied)
    {
        int attackID = getNewAttack();
        useAttack(attackID, commandQueue);
    }

    ///for(auto& attack : mCurrentAttacks)
        ///attack->updateCurrent(dt, commandQueue); // ensures that attacks are updated after AttackManager and not earlier(could happen while updating scene graph)

    clearFinishedAttacks();
    commandQueue.push(mTargetsCollector);
}

int AttackManager::getNewAttack() const
{
    int range1 = 0, range2 = 0, num = randomInt(1, 100);

    for(const auto& attack : mAttacks)
    {
        range2 += attack.second; // probability

        if(num > range1 && num <= range2)
            return attack.first; // id

        range1 += attack.second;
    }

    return -1;
}

void AttackManager::updatePosition(sf::Vector2f position)
{
    sf::Vector2f displacement = position - mPosition;

    for(auto& attack : mCurrentAttacks)
        attack->updateBarrierPosition(displacement);

    mPosition = position;
}

void AttackManager::clearFinishedAttacks()
{
    auto newBeg = std::remove_if(mCurrentAttacks.begin(), mCurrentAttacks.end(), [](Attack* attack)
    {
        if(!attack->isActive())
        {
            attack->markForRemoval();
            return true;
        }
        return false;
    });

    mCurrentAttacks.erase(newBeg, mCurrentAttacks.end());
}

void AttackManager::initializeCommands()
{
    if(mIsAllied)
        mTargetsCollector.mCategories.push_back(Category::EnemyAircraft);
    else
        mTargetsCollector.mCategories.push_back(Category::PlayerAircraft);

    mTargetsCollector.mAction = castFunctor<Aircraft>([this](Aircraft& target, sf::Time dt)
    {
        if(!target.isMarkedForRemoval())
            mPossibleTargets.push_back(&target);
    });
}
