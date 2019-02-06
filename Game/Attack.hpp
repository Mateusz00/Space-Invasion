#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "Projectile.hpp"
#include "Entity.hpp"
#include "ResourcesID.hpp"
#include "Command.hpp"
#include <SFML/System/Time.hpp>
#include <vector>
class CommandQueue;
class World;
class AttackManager;

class Attack : public Entity
{
    public:
                                Attack(int id, const TextureHolder&, World&, AttackManager&);
        virtual                 ~Attack();
        void                    update(sf::Time, CommandQueue&);
        bool                    isActive() const;
        virtual Category::Type  getCategory() const override;
        virtual sf::FloatRect   getBoundingRect() const override;

    private:
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        virtual bool            isMarkedForRemoval() const override;
        void                    activate();
        void                    deactivate();
        void                    createProjectile(SceneNode& layer, int num);

        std::vector<std::unique_ptr<Projectile*>>   mProjectiles;
        int                                         mAttackID;
        bool                                        mIsActive;
        sf::Vector2f                                mPosition;
        Command                                     mAttackCommand;
        AttackManager&                              mAttackManager;
};

#endif // ATTACK_HPP
