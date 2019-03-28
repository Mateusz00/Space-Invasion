#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include "../ResourcesID.hpp"
#include "../Command.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
class CommandQueue;
class Attack;
class Aircraft;
class World;

class AttackManager
{
    public:
        explicit    AttackManager(const TextureHolder&, World& world, int shooterID,
                                   bool isAllied, const std::vector<Aircraft*>& targets);
        void        pushAttack(int id, int probability);
        void        forceAttack(int id, CommandQueue&);
        sf::Time    getCooldown() const;
        void        forceCooldown(sf::Time);
        bool        tryAttack(int id, CommandQueue&);
        void        update(sf::Time, CommandQueue&);
        void        updatePosition(sf::Vector2f);
        void        onRemoval();

    private:
        void        clearFinishedAttacks();
        int         getNewAttack();
        void        useAttack(int id, CommandQueue&, bool applyCooldown = true);

        std::vector<std::pair<int, int>>    mAttacks; // id, probability
        std::vector<Attack*>                mCurrentAttacks;
        sf::Time                            mCooldown;
        const TextureHolder&                mTextures;
        sf::Vector2f                        mPosition;
        World&                              mWorld;
        int                                 mShooterID;
        bool                                mIsAllied;
        std::unordered_map<int, int>        mRepeats; // id, times
        std::unordered_map<int, sf::Time>   mRepeatCooldowns;
        std::unordered_map<int, sf::Time>   mChargingAttacks;
        const std::vector<Aircraft*>&       mTargets;
};

#endif // ATTACKMANAGER_HPP
