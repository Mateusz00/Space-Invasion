#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP

#include "../SceneNode.hpp"
#include "Entity.hpp"
#include "Aircraft.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class HealthBar : public SceneNode
{
    public:
                        HealthBar(Entity&, sf::Vector2f dimensions, int maxHitpoints);
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        void            setPosition(sf::Vector2f position);
        void            setPosition(float x, float y);

    private:
        sf::RectangleShape  mHealthBar;
        float               mMaxValue;
        sf::Vector2f        mMaxSize;
        Entity&             mObject;
};

#endif // HEALTHBAR_HPP
