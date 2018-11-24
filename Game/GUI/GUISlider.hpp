#ifndef GUISLIDER_HPP
#define GUISLIDER_HPP

#include "GUIObject.hpp"
#include "GUIContainer.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class GUISlider : public GUIObject
{
    public:
                                GUISlider(float width, float height, float currentValue, FontHolder&, GUIContainer* = nullptr);
                                GUISlider(sf::Vector2f, float currentValue, FontHolder&, GUIContainer* = nullptr);
        virtual void            deactivate() override;
        virtual bool            isSelectable() const override;
        virtual void            handleEvent(const sf::Event&) override;
        virtual sf::FloatRect   getBoundingRect() const override;
        virtual void            onMouseClick(sf::Vector2i) override;
        void                    setUpdatingFunction(std::function<void(float)>);
        void                    setValueRange(float minValue, float maxValue);
        void                    setValueVisibility(bool showValue);

    private:
        virtual void            draw(sf::RenderTarget&, sf::RenderStates) const override;
        void                    changeSliderPosition(int mouseXPosition);

        sf::RectangleShape          mSlider;
        sf::RectangleShape          mBar;
        float                       mValue;
        float                       mMaxValue;
        float                       mMinValue;
        sf::Text                    mText;
        GUIContainer*               mContainer;
        bool                        mShowValue;
        std::function<void(float)>  mUpdatingFunction;
};

#endif // GUISLIDER_HPP
