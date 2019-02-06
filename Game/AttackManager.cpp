#include "AttackManager.hpp"
#include "CommandQueue.hpp"

AttackManager::AttackManager(const TextureHolder& textures, World& world)
    : mTextures(textures),
      mWorld(world)
{
}

void AttackManager::pushAttack(int id, int probability)
{
    mAttacks.push_back(std::make_pair(id, probability));
}

void AttackManager::useAttack()
{

}

void AttackManager::forceAttack(int id) // For players attacks
{

}

void AttackManager::forceCooldown(sf::Time cooldown)
{
    mCooldown = cooldown;
}

void AttackManager::update(sf::Time dt, CommandQueue commandQueue)
{
    mCurrentAttacks.cl

    for(auto& attack : mCurrentAttacks)
        attack.update(dt, commandQueue);
    // First delete finished attacks then let attack send commands
}

void AttackManager::updatePosition(sf::Vector2f position)
{
    mPosition = position;
}

void AttackManager::clearFinishedAttacks()
{

}
