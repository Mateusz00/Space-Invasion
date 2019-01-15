#ifndef TEXTBAR_HPP
#define TEXTBAR_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

class TextBar : public sf::Drawable, public sf::Transformable
{
    public:
                        TextBar(const sf::Font&, const std::string&, unsigned int charSize, bool fadeOverTime=true);
        bool            isActive() const;
        void            setActive(bool);
        void            setFadeTime(sf::Time); /// Throws logic_error if time is negative
        void            setFadeCooldown(sf::Time); /// Throws logic_error if time is negative
        void            setTextFillColor(sf::Color);
        void            setTextOutlineColor(sf::Color);
        void            setBarFillColor(sf::Color);
        void            setBarOutlineColor(sf::Color);
        void            setBarSize(sf::Vector2f);
        sf::Vector2f    getBarSize() const;
        void            update(sf::Time);
        void            centerOrigin();

    private:
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override;

        sf::RectangleShape      mRect;
        sf::Text                mText;
        sf::Time                mFadeTime;
        sf::Time                mFadeCooldown;
        sf::Time                mFadeCountdown;
        bool                    mFadeOverTime;
        bool                    mIsActive;
        sf::Color               mRectFill;
        sf::Color               mRectOutline;
        sf::Color               mTextFill;
        sf::Color               mTextOutline;
};

#endif // TEXTBAR_HPP
