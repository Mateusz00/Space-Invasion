#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "Entity.hpp"
#include "../ResourcesID.hpp"
#include "../Category.hpp"
#include "../Command.hpp"
#include <SFML/Graphics/Sprite.hpp>
class Spaceship;

class Pickup : public Entity
{
    public:
        enum Type
        {
            FireRate,
            FireSpread,
            MissileRefill,
            HealthRefill,
            TypeCount
        };

                                Pickup(Type, const TextureHolder&, World&);
        virtual Category::Type  getCategory() const override;
        virtual sf::FloatRect   getBoundingRect() const override;
        void                    apply(Spaceship&) const;
        virtual void            onCollision(Entity&) override;

    private:
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        Type             mType;
        sf::Sprite       mSprite;
};

#endif // PICKUP_HPP
