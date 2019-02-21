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
    mAttacks.push_back(std::make_pair(id, probability));
}

void AttackManager::useAttack(int id, CommandQueue& commands)
{
    Command launchAttack;
    launchAttack.mCategories.push_back(Category::AirLayer);
    launchAttack.mAction = [this, id](SceneNode& layer, sf::Time)
    {
        std::unique_ptr<Attack> attack(new Attack(id, mTextures, mPosition, mWorld, mShooterID, mPossibleTargets));
        layer.attachChild(std::move(attack));
    };

    commands.push(launchAttack);
}

void AttackManager::forceAttack(int id, CommandQueue& commands) // For players attacks
{
    useAttack(id, commands);
}

void AttackManager::forceCooldown(sf::Time cooldown)
{
    mCooldown = cooldown;
}

void AttackManager::update(sf::Time dt, CommandQueue& commandQueue)
{
    int attackID;

    if(mCooldown <= sf::Time::Zero)
    {
        attackID = getNewAttack();
        useAttack(attackID, commandQueue);
        mCooldown += Attacks::attackData.at(attackID).cooldown;
    }

    for(auto& attack : mCurrentAttacks)
        attack->update(dt, commandQueue); // ensures that attacks are updated after AttackManager and not earlier(could happen while updating scene graph)

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

    return 0;
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
    for(auto attack = mCurrentAttacks.begin(); attack != mCurrentAttacks.end(); ++attack)
    {
        if(!(*attack)->isActive())
        {
            (*attack)->markForRemoval();
            mCurrentAttacks.erase(attack);
        }
    }
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
