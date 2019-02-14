#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "../Entities/Projectile.hpp"
#include "../Entities/Entity.hpp"
#include "../ResourcesID.hpp"
#include "../Command.hpp"
#include <SFML/System/Time.hpp>
#include <vector>
class CommandQueue;
class World;

class Attack : public Entity
{
    public:
        enum Behavior
        {
            StraightLine,
            Guided,
            Spiral,
            Orbiting,
            Barrier
        };

                                Attack(int id, const TextureHolder&, sf::Vector2f pos, World&, int shooterID);
        virtual                 ~Attack();
        void                    update(sf::Time, CommandQueue&);
        bool                    isActive() const;
        virtual Category::Type  getCategory() const;
        virtual sf::FloatRect   getBoundingRect() const override; // AttackManager checks if attack isn't visible and deletes it
        virtual void            removeEntity();
        virtual bool            isMarkedForRemoval() const override; // AttackManager checks it
        void                    markForRemoval();
        bool                    isBarrier() const;
        void                    updatePosition(sf::Vector2f);

    private:
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        void                    activate();
        void                    deactivate();
        void                    createProjectile(int num);

        std::vector<std::unique_ptr<Projectile>>    mProjectiles;
        int                                         mAttackID;
        const TextureHolder&                        mTextures;
        bool                                        mIsActive;
        bool                                        mIsReadyToDelete;
        sf::Vector2f                                mPosition;
        int                                         mShooterID;
};

#endif // ATTACK_HPP
