#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "Entity.hpp"
#include "../Category.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Aircraft : public Entity
{
    public:
        enum Type{Ally, Enemy, TypeCount};

        Aircraft(Type, const TextureHolder&, const FontHolder&);
        virtual void updateCurrent(sf::Time) override;
        virtual void drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        Category::Type getCategory();
        void increaseFireRate();
        void increaseSpread();
        int  getMissileAmmo();
        void setMissileAmmo(int);
        int  getIdentifier();
        void setIdentifier(int);
        void fire();
        void launchMissile();

    private:
        Type mType;
        sf::Sprite mSprite;
        int	mFireRateLevel;
        int	mSpreadLevel;
        int	mMissileAmmo;
        int mIdentifier;
};

#endif // AIRCRAFT_HPP
