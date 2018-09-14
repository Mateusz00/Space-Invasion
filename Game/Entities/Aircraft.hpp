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
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        Category::Type  getCategory() const override;
        void            increaseFireRate();
        void            increaseSpread();
        int             getMissileAmmo() const;
        void            setMissileAmmo(int);
        int             getIdentifier() const;
        void            setIdentifier(int);
        void            fire();
        void            launchMissile();
        float           getMaxSpeed() const;

    private:
        Type        mType;
        sf::Sprite  mSprite;
        int	        mFireRateLevel;
        int	        mSpreadLevel;
        int	        mMissileAmmo;
        int         mIdentifier;
};

#endif // AIRCRAFT_HPP
