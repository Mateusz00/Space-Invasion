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

                                Projectile(Type, const TextureHolder&);
        float					getMaxSpeed() const;
        int						getDamage() const;
        bool					isGuided() const;
        void	                guideTowards(sf::Vector2f);
        virtual Category::Type  getCategory() const override;
        virtual sf::FloatRect   getBoundingRect() const override;

    private:
        virtual void updateCurrent(sf::Time, CommandQueue&) override;
        virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        Type            mType;
        sf::Sprite      mSprite;
        sf::Vector2f    mTargetDirection;
};

#endif // PROJECTILE_HPP
