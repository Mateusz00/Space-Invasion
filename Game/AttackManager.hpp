#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include "Attack.hpp"
#include "ResourcesID.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
class CommandQueue;

class AttackManager
{
    public:
        explicit    AttackManager(const TextureHolder&);
        void        pushAttack(int id, int probability);
        void        useAttack();
        void        forceAttack(int id);
        void        forceCooldown(sf::Time);
        void        update(sf::Time, CommandQueue&); // Not used
        void        updatePosition(sf::Vector2f);

    private:
        void        clearFinishedAttacks();

        std::vector<std::pair<int, int>>    mAttacks; // id, probability
        std::vector<Attack>                 mCurrentAttacks;
        sf::Time                            mCooldown;
        const TextureHolder&                mTextures;
        sf::Vector2f                        mPosition;
};

#endif // ATTACKMANAGER_HPP
