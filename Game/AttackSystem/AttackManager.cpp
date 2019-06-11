#include "AttackManager.hpp"
#include "Attack.hpp"
#include "../Entities/Spaceship.hpp"
#include "../DataTable.hpp"
#include "../CommandQueue.hpp"
#include "../Utility.hpp"
#include "../World.hpp"
#include "Attacks.hpp"

AttackManager::AttackManager(const TextureHolder& textures, ObjectContext context, int shooterID,
                              bool isAllied, const std::vector<Spaceship*>& targets)
    : mTextures(textures),
      mCooldown(sf::seconds(0.1f)),
      mContext(context),
      mShooterID(shooterID),
      mIsAllied(isAllied),
      mTargets(targets)
{
}

void AttackManager::pushAttack(int id, int probability)
{
    mAttacks.emplace_back(id, probability);
}

void AttackManager::useAttack(int id, CommandQueue& commands, bool applyCooldown, int phase)
{
    if(id == -1)
        return;

    auto& attack = Attacks::attackData.at(id);

    if(attack.phaseQueue.size() > 0)
        mPhaseManagers.emplace_back(PhaseManager{sf::Time::Zero, &attack.phaseQueue, 0, 0});

    if(applyCooldown)
        mCooldown += attack.cooldown;
}

void AttackManager::launchAttack(int id, CommandQueue& commands, int phase)
{
    Command launchAttack;
    launchAttack.mCategories = Category::AirLayer;
    launchAttack.mAction = [this, id, phase](SceneNode& layer, sf::Time)
    {
        std::unique_ptr<Attack> attack(new Attack(id, mTextures, mPosition, mContext, mShooterID, mIsAllied, mTargets, phase));
        mCurrentAttacks.emplace_back(attack.get());
        layer.attachChild(std::move(attack));
    };

    commands.push(launchAttack);
}

void AttackManager::forceAttack(int id, CommandQueue& commands, bool applyCooldown)
{
    if(analyzeAttack(id) > -1)
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
    if(mCooldown <= sf::Time::Zero && analyzeAttack(id) > -1)
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

    for(std::pair<const int, sf::Time>& attack : mChargingAttacks)
        attack.second = std::max(attack.second - dt, sf::Time::Zero);

    for(PhaseManager& manager : mPhaseManagers)
        manager.cooldown = std::max(manager.cooldown - dt, sf::Time::Zero);

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
            useAttack(attack.first, commandQueue, false);
    }

    // Manage attack phases
    for(PhaseManager& manager : mPhaseManagers)
    {
        if(manager.cooldown <= sf::Time::Zero)
        {
            int attackID = (*manager.phaseQueue)[manager.currentPhase].attackID;
            int phase = (*manager.phaseQueue)[manager.currentPhase].phaseID;
            launchAttack(attackID, commandQueue, phase);
            manager.currentRepeat++;

            int repeats = Attacks::attackData.at(attackID).phases[phase].repeats;
            if(manager.currentRepeat < repeats)
            {
                manager.cooldown += Attacks::attackData.at(attackID).phases[phase].repeatCooldown;
            }
            else
            {
                manager.cooldown += (*manager.phaseQueue)[manager.currentPhase].phaseCooldown;
                manager.currentRepeat = 0;
                manager.currentPhase++;
            }
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
            return analyzeAttack(attack.first);

        range1 += attack.second;
    }

    return -1;
}

/// Checks if attack should be repeated n times or if it's charged. Puts attack on corresponding attack list if needed.
///@return -1 if attack should be used in non-standard way(has repeats or is charged attack)
int AttackManager::analyzeAttack(int id)
{
    if(Attacks::attackData.at(id).chargingTime != sf::Time::Zero)
    {
        mCooldown += Attacks::attackData.at(id).cooldown;
        mChargingAttacks.emplace(id, Attacks::attackData.at(id).chargingTime);
        return -1;
    }

    return id;
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

    for(auto it = mChargingAttacks.begin(); it != mChargingAttacks.end() && !mChargingAttacks.empty();)
    {
        if(it->second <= sf::Time::Zero)
            mChargingAttacks.erase(it->first);
        else
            ++it;
    }

    auto newBeg2 = std::remove_if(mPhaseManagers.begin(), mPhaseManagers.end(), [](PhaseManager& manager)
    {
        if(manager.currentPhase >= manager.phaseQueue->size())
            return true;

        return false;
    });

    mCurrentAttacks.erase(newBeg, mCurrentAttacks.end());
    mPhaseManagers.erase(newBeg2, mPhaseManagers.end());
}

void AttackManager::onRemoval()
{
    for(auto& attack : mCurrentAttacks)
        attack->markForRemoval();
}
