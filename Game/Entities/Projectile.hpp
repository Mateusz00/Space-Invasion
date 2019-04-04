#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "../ResourcesID.hpp"
#include "../AttackSystem/AttackPattern.hpp"
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

                                        Projectile(Type, const TextureHolder&, World&, int shooterID, float speed);
        float                           getMaxSpeed() const;
        int                             getDamage() const;
        virtual Category::Type          getCategory() const override;
        virtual sf::FloatRect           getBoundingRect() const override;
        virtual void                    onCollision(Entity&) override;
        int                             getShootersID() const;
        void                            setPattern(AttackPattern::ID);
        void                            setPatternData(AttackPattern::PatternData);
        AttackPattern::ID               getPattern() const;
        AttackPattern::PatternData      getPatternData() const;
        void                            setDirection(sf::Vector2f);
        sf::Vector2f                    getDirection() const;
        void                            setStartPos(sf::Vector2f);
        sf::Vector2f                    getStartPos() const;
        float                           activeTime() const;
        void                            updateTime(sf::Time dt);

    private:
        virtual void            updateCurrent(sf::Time, CommandQueue&) override;
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        Type                            mType;
        sf::Sprite                      mSprite;
        int                             mShooterID;
        AttackPattern::ID               mPattern;
        AttackPattern::PatternData      mPatternData;
        float                           mSpeed;
        sf::Vector2f                    mDirection;
        sf::Vector2f                    mStartPos;
        sf::Time                        mTimeActive;
};

#endif // PROJECTILE_HPP
