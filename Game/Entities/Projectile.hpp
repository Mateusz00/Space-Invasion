#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
    public:
        enum Type
        {
            AlliedBullet,
            EnemyBullet,
            Missile,
            TypeCount
        };

                                Projectile(Type, const TextureHolder&, World&, int shooterID);
        float                   getMaxSpeed() const;
        int                     getDamage() const;
        bool                    isGuided() const;
        void                    guideTowards(sf::Vector2f);
        virtual Category::Type  getCategory() const override;
        virtual sf::FloatRect   getBoundingRect() const override;
        virtual void            onCollision(Entity&) override;
        int                     getShootersID() const;
        void                    setBehavior(int);

    private:
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        Type            mType;
        sf::Sprite      mSprite;
        sf::Vector2f    mTargetDirection;
        int             mShooterID;
        float           mSpeed; // TODO: Initialize it in ctor
};

#endif // PROJECTILE_HPP
