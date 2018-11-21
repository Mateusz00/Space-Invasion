#ifndef GUISLIDER_HPP
#define GUISLIDER_HPP

#include "GUIObject.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class GUISlider : public GUIObject
{
    public:
                                GUISlider(sf::Vector2f size, float minValue, float maxValue, float& out, FontHolder&);
        virtual void            deactivate() override;
        virtual bool            isSelectable() const override;
        virtual void            handleEvent(const sf::Event&) override;
        virtual sf::FloatRect   getBoundingRect() const override;
        virtual void            onMouseClick(sf::Vector2i) override;

    private:
        virtual void            draw(sf::RenderTarget&, sf::RenderStates) const override;
        void                    changeSliderPosition(int mouseXPosition);

        sf::RectangleShape  mSlider;
        sf::RectangleShape  mBar;
        float&              mValue;
        float               mMaxValue;
        float               mMinValue;
        sf::Text            mText;
};

#endif // GUISLIDER_HPP
