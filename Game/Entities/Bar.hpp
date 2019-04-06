#ifndef BAR_HPP
#define BAR_HPP

#include "../SceneNode.hpp"
#include "Entity.hpp"
#include "Aircraft.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class Bar : public SceneNode
{
    public:
                        Bar(float val, float maxValue, sf::Vector2f maxSize);
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;
        virtual void    updateCurrent(sf::Time, CommandQueue&) override;
        void            setPosition(sf::Vector2f position);
        void            setPosition(float x, float y);
        void            updateValue(float);

    private:
        sf::RectangleShape  mBar;
        float               mMaxValue;
        float               mCurrentValue;
        sf::Vector2f        mMaxSize;
};

#endif // BAR_HPP
