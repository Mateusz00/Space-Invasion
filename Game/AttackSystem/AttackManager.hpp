#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include "../ResourcesID.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
class CommandQueue;
class Attack;
class World;

class AttackManager
{
    public:
        explicit    AttackManager(const TextureHolder&, World& world, int);
        void        pushAttack(int id, int probability);
        void        forceAttack(int id, CommandQueue&);
        void        forceCooldown(sf::Time);
        void        update(sf::Time, CommandQueue&); // Not used
        void        updatePosition(sf::Vector2f);

    private:
        void        clearFinishedAttacks();
        int         getNewAttack() const;
        void        useAttack(int id, CommandQueue&);

        std::vector<std::pair<int, int>>    mAttacks; // id, probability
        std::vector<Attack*>                mCurrentAttacks;
        sf::Time                            mCooldown;
        const TextureHolder&                mTextures;
        sf::Vector2f                        mPosition;
        World&                              mWorld;
        int                                 mShooterID;
};

#endif // ATTACKMANAGER_HPP
