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
        explicit    AttackManager(const TextureHolder&, World& world, int shooterID, bool isAllied);
        void        pushAttack(int id, int probability);
        void        forceAttack(int id, CommandQueue&);
        void        forceCooldown(sf::Time);
        void        update(sf::Time, CommandQueue&); // Not used
        void        updatePosition(sf::Vector2f);

    private:
        void        clearFinishedAttacks();
        int         getNewAttack() const;
        void        useAttack(int id, CommandQueue&);
        void        initializeCommands();

        std::vector<std::pair<int, int>>    mAttacks; // id, probability
        std::vector<Aircraft*>              mPossibleTargets;
        std::vector<Attack*>                mCurrentAttacks;
        sf::Time                            mCooldown;
        const TextureHolder&                mTextures;
        sf::Vector2f                        mPosition;
        World&                              mWorld;
        int                                 mShooterID;
        bool                                mIsAllied;
        Command                             mTargetsCollector;
};

#endif // ATTACKMANAGER_HPP
