#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "GravityCenter.hpp"
#include "../Entities/Projectile.hpp"
#include "../Entities/Entity.hpp"
#include "../ResourcesID.hpp"
#include "../Command.hpp"
#include "../Category.hpp"
#include <SFML/System/Time.hpp>
#include <vector>
class CommandQueue;
class Aircraft;
class World;

class Attack : public Entity
{
    public:
                                Attack(int id, const TextureHolder&, sf::Vector2f pos, World&, int shooterID,
                                        bool isAllied, const std::vector<Aircraft*>& targets);
        void                    updateCurrent(sf::Time, CommandQueue&);
        bool                    isActive() const;
        virtual Category::Type  getCategory() const override;
        virtual sf::FloatRect   getBoundingRect() const override; // AttackManager checks if attack isn't visible and deletes it
        virtual void            removeEntity() override;
        virtual bool            isMarkedForRemoval() const override; // AttackManager checks it
        void                    markForRemoval();
        void                    updateBarrierPosition(sf::Vector2f displacement);///
        bool                    isAllied() const;

    private:
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        void                    activate();
        void                    deactivate();
        void                    createProjectiles();
        void                    createProjectile(int num);
        void                    createGravityCenters();
        void                    createGravityCenter(int num);
        void                    applyDisplacement(int gravityCenterID, sf::Vector2f);
        sf::Vector2f            getClosestTarget(const sf::Transformable*) const;

        std::vector<std::unique_ptr<Projectile>>    mProjectiles;
        std::unordered_map<int, GravityCenter>      mGravityCenters;
        const std::vector<Aircraft*>&               mTargets;
        int                                         mAttackID;
        const TextureHolder&                        mTextures;
        bool                                        mIsActive;
        bool                                        mIsReadyToDelete;
        bool                                        mIsAllied;
        bool                                        mWasCreated;
        sf::Vector2f                                mPosition;
        int                                         mShooterID;
};

#endif // ATTACK_HPP
