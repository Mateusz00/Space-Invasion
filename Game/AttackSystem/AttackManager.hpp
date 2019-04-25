#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include "../ResourcesID.hpp"
#include "../Command.hpp"
#include "../DataTable.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
class CommandQueue;
class Attack;
class Spaceship;
class World;

class AttackManager
{
    public:
        explicit    AttackManager(const TextureHolder&, World& world, int shooterID,
                                   bool isAllied, const std::vector<Spaceship*>& targets);
        void        pushAttack(int id, int probability);
        void        forceAttack(int id, CommandQueue&, bool applyCooldown);
        sf::Time    getCooldown() const;
        void        forceCooldown(sf::Time);
        bool        tryAttack(int id, CommandQueue&);
        void        update(sf::Time, CommandQueue&);
        void        updatePosition(sf::Vector2f);
        void        onRemoval();

    private:
        using Phases = const std::vector<AttackData::PhaseInfo>;
        struct PhaseManager
        {
            sf::Time    cooldown;
            Phases*     phaseQueue; // attackID, phase
            int         currentPhase;
            int         currentRepeat;
        };

        void        clearFinishedAttacks();
        int         getNewAttack();
        int         analyzeAttack(int id);
        void        useAttack(int id, CommandQueue&, bool applyCooldown = true, int phase = 0);
        void        launchAttack(int id, CommandQueue&, int phase = 0);

        std::vector<std::pair<int, int>>    mAttacks; // id, probability
        std::vector<Attack*>                mCurrentAttacks;
        sf::Time                            mCooldown;
        const TextureHolder&                mTextures;
        sf::Vector2f                        mPosition;
        World&                              mWorld;
        int                                 mShooterID;
        bool                                mIsAllied;
        std::unordered_map<int, sf::Time>   mChargingAttacks;
        const std::vector<Spaceship*>&      mTargets;
        std::vector<PhaseManager>           mPhaseManagers;
};

#endif // ATTACKMANAGER_HPP
