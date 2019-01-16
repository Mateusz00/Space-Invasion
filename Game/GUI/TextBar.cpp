#include "TextBar.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <stdexcept>

TextBar::TextBar(const sf::Font& font, const std::string& text, unsigned int charSize, bool fadeOverTime)
    : mText(text, font, charSize),
      mFadeOverTime(fadeOverTime),
      mIsActive(false)
{
    centerText(mRect, mText);
}

bool TextBar::isActive() const
{
    return mIsActive;
}

void TextBar::setActive(bool active)
{
    mText.setFillColor(mTextFill);
    mText.setOutlineColor(mTextOutline);
    mRect.setFillColor(mRectFill);
    mRect.setOutlineColor(mRectOutline);
    mFadeCountdown = sf::Time::Zero;
    mIsActive = active;
}

void TextBar::setFadeTime(sf::Time time)
{
    if(time >= sf::Time::Zero)
        mFadeTime = time;
    else
        throw std::logic_error("Time can't be negative! (TextBar)");
}

void TextBar::setFadeCooldown(sf::Time time)
{
    if(time >= sf::Time::Zero)
        mFadeCooldown = time;
    else
        throw std::logic_error("Time can't be negative! (TextBar)");
}

void TextBar::setTextFillColor(sf::Color color)
{
    mText.setFillColor(color);
    mTextFill = color;
}

void TextBar::setTextOutlineColor(sf::Color color)
{
    mText.setOutlineColor(color);
    mTextOutline = color;
}

void TextBar::setBarFillColor(sf::Color color)
{
    mRect.setFillColor(color);
    mRectFill = color;
}

void TextBar::setBarOutlineColor(sf::Color color)
{
    mRect.setOutlineColor(color);
    mRectOutline = color;
}

void TextBar::setBarSize(sf::Vector2f barSize)
{
    mRect.setSize(barSize);
}

sf::Vector2f TextBar::getBarSize() const
{
    return mRect.getSize();
}

void TextBar::update(sf::Time dt)
{
    if(mIsActive)
    {
        mFadeCountdown += dt;

        if(mFadeCountdown > mFadeCooldown)
        {
            float mVisibilityPercentage = 1.f - ((mFadeCountdown-mFadeCooldown) / mFadeTime);

            if(mVisibilityPercentage >= 0.f) // Change transparency only for <1, 0> visibility percentage
            {
                sf::Color textFill      = mTextFill;
                sf::Color textOutline   = mTextOutline;
                sf::Color rectFill      = mRectFill;
                sf::Color rectOutline   = mRectOutline;

                textFill.a      *= mVisibilityPercentage;
                textOutline.a   *= mVisibilityPercentage;
                rectFill.a      *= mVisibilityPercentage;
                rectOutline.a   *= mVisibilityPercentage;

                mText.setFillColor(textFill);
                mText.setOutlineColor(textOutline);
                mRect.setFillColor(rectFill);
                mRect.setOutlineColor(rectOutline);
            }
            else
                setActive(false);
        }
    }
}

void TextBar::centerOrigin()
{
    ::centerOrigin(mRect); // Use function from Utility.hpp
    ::centerOrigin(mText);
}

void TextBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(mIsActive)
    {
        states.transform *= getTransform();
        target.draw(mRect, states);
        target.draw(mText, states);
    }
}
