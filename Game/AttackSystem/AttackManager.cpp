#include "AttackManager.hpp"
#include "Attack.hpp"
#include "../Entities/Spaceship.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
#include "../World.hpp"
#include "Attacks.hpp"

AttackManager::AttackManager(const TextureHolder& textures, World& world, int shooterID,
                              bool isAllied, const std::vector<Spaceship*>& targets)
    : mTextures(textures),
      mCooldown(sf::seconds(0.1f)),
      mWorld(world),
      mShooterID(shooterID),
      mIsAllied(isAllied),
      mTargets(targets)
{
}

void AttackManager::pushAttack(int id, int probability)
{
    mAttacks.emplace_back(id, probability);
}

void AttackManager::useAttack(int id, CommandQueue& commands, bool applyCooldown)
{
    if(id == -1)
        return;

    Command launchAttack;
    launchAttack.mCategories.push_back(Category::AirLayer);
    launchAttack.mAction = [this, id](SceneNode& layer, sf::Time)
    {
        std::unique_ptr<Attack> attack(new Attack(id, mTextures, mPosition, mWorld, mShooterID, mIsAllied, mTargets));
        mCurrentAttacks.emplace_back(attack.get());
        layer.attachChild(std::move(attack));
    };

    commands.push(launchAttack);

    if(applyCooldown)
        mCooldown += Attacks::attackData.at(id).cooldown;
}

void AttackManager::forceAttack(int id, CommandQueue& commands, bool applyCooldown)
{
    useAttack(id, commands, applyCooldown);
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
    // Update cool-downs
    mCooldown = std::max(mCooldown - dt, sf::Time::Zero);

    for(std::pair<const int, sf::Time>& repeatCooldown : mRepeatCooldowns)
        repeatCooldown.second = std::max(repeatCooldown.second - dt, sf::Time::Zero);

    for(std::pair<const int, sf::Time>& attack : mChargingAttacks)
        attack.second = std::max(attack.second - dt, sf::Time::Zero);

    // Get new attack if possible
    if(mCooldown <= sf::Time::Zero && !mIsAllied)
    {
        int attackID = getNewAttack();
        useAttack(attackID, commandQueue);
    }

    // Use attacks that have been charged
    for(std::pair<const int, sf::Time>& attack : mChargingAttacks)
    {
        if(attack.second <= sf::Time::Zero)
        {
            if(Attacks::attackData.at(attack.first).repeats > 0)
                initiateRepeatedAttack(attack.first);
            else
                useAttack(attack.first, commandQueue, false);
        }
    }

    // Manage repeated attacks
    for(std::pair<const int, int>& repeatPair : mRepeats)
    {
        if(mRepeatCooldowns.at(repeatPair.first) <= sf::Time::Zero)
        {
            useAttack(repeatPair.first, commandQueue, false);
            mRepeatCooldowns[repeatPair.first] += Attacks::attackData.at(repeatPair.first).repeatCooldown;
            --repeatPair.second;
        }
    }

    clearFinishedAttacks();
}

int AttackManager::getNewAttack()
{
    int range1 = 0, range2 = 0, num = randomInt(1, 100);

    for(const auto& attack : mAttacks)
    {
        range2 += attack.second; // probability

        if(num > range1 && num <= range2)
        {
            if(Attacks::attackData.at(attack.first).chargingTime == sf::Time::Zero)
            {
                if(Attacks::attackData.at(attack.first).repeats > 0)
                {
                    initiateRepeatedAttack(attack.first);
                    return -1;
                }

                return attack.first; // id
            }
            else
            {
                mCooldown += Attacks::attackData.at(attack.first).cooldown;
                mChargingAttacks.emplace(attack.first, Attacks::attackData.at(attack.first).chargingTime);
                return -1;
            }
        }

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

    for(auto it = mRepeats.begin(); it != mRepeats.end() && !mRepeats.empty();)
    {
        if(it->second <= 0) // repeats
        {
            mRepeatCooldowns.erase(it->first);
            mRepeats.erase(it);
        }
        else
            ++it;
    }

    for(auto it = mChargingAttacks.begin(); it != mChargingAttacks.end() && !mChargingAttacks.empty();)
    {
        if(it->second <= sf::Time::Zero)
            mChargingAttacks.erase(it->first);
        else
            ++it;
    }

    mCurrentAttacks.erase(newBeg, mCurrentAttacks.end());
}

void AttackManager::onRemoval()
{
    for(auto& attack : mCurrentAttacks)
        attack->markForRemoval();
}

void AttackManager::initiateRepeatedAttack(int id)
{
    mRepeats[id] = Attacks::attackData.at(id).repeats;
    mRepeatCooldowns.emplace(id, sf::Time::Zero);
}
