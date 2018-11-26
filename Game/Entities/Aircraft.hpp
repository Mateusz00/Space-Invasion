#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "Projectile.hpp"
#include "../Category.hpp"
#include "../Command.hpp"
#include "../CommandQueue.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
class Entity;

class Aircraft : public Entity
{
    public:
        enum Type{Ally, Enemy, TypeCount};

                                Aircraft(Type, const TextureHolder&, const FontHolder&, World&, int id = 600);
        Category::Type          getCategory() const override;
        void                    increaseFireRate();
        void                    increaseSpread();
        int                     getMissileAmmo() const;
        void                    setMissileAmmo(int);
        void                    changeMissileAmmo(int);
        int                     getIdentifier() const;
        void                    setIdentifier(int);
        void                    fire();
        void                    launchMissile();
        float                   getMaxSpeed() const;
        virtual sf::FloatRect   getLocalBounds() const override;
        virtual sf::FloatRect   getBoundingRect() const override;
        virtual void            onCollision(Entity&) override;
        virtual void            removeEntity() override;
        void                    increaseScore(int);
        int                     getScore() const;
        void                    setAttackerID(int);

    protected:
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

    private:
        void            updateRollAnimation(sf::Time);
        void            shootBullets(SceneNode&, const TextureHolder&) const;
        void            createProjectile(SceneNode&, Projectile::Type, float xOffset, float yOffset, const TextureHolder&) const;
        void            launchProjectiles(sf::Time, CommandQueue&);
        void            updateMovementPatterns(sf::Time);
        void            createPickup() const;
        void            createExplosion() const;
        void            changeScore();
        void            increaseScoreRequest(int) const;
        void            decreaseScoreRequest(int) const;
        virtual void    onRemoval() override;

        Type                    mType;
        sf::Sprite              mSprite;
        int                     mFireRateLevel;
        int                     mSpreadLevel;
        int                     mMissileAmmo;
        int                     mIdentifier;
        bool                    mIsFiring;
        bool                    mIsLaunchingMissile;
        bool                    mIsEnemy;
        bool                    mShowExplosion;
        float                   mTravelledDistance;
        int                     mDirectionIndex;
        sf::Time                mFireCooldown;
        sf::Time                mLastRoll;
        Command                 mFireCommand;
        Command                 mLaunchMissileCommand;
        const TextureHolder&    mTextures;
        int                     mAttackerID;
        int                     mScore;
};

#endif // AIRCRAFT_HPP
