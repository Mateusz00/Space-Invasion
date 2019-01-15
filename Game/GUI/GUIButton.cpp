#include "GUIButton.hpp"
#include "../SoundPlayer.hpp"
#include "../ResourcesID.hpp"
#include "../Utility.hpp"
#include "../DataTable.hpp"
#include <SFML/System/Vector2.hpp>

namespace
{
    const std::vector<ButtonData> table = initializeButtonData();
}

GUIButton::GUIButton(State::Context context, ButtonID id, const std::string& text, bool locked)
    : mButtonID(id),
      mButtonType(table[id].buttonType),
      mText(text, context.fonts.get(Fonts::Sansation), 26u),
      mIsToggled(false),
      mFreezeAppearance(false),
      mIsLocked(locked),
      mSounds(context.sounds)
{
    if(!mIsLocked)
        changeAppearance(ButtonState::Normal);
    else
        changeAppearance(ButtonState::Locked);

    switch(mButtonType)
    {
        case ButtonType::Textured:
            mSprite.setTexture(context.textures.get(table[mButtonID].textureId));
            centerText(mSprite, mText);
            break;

        case ButtonType::SimpleRect:
            centerText(mBox, mText);
            break;
    }

    if(mButtonType != ButtonType::Text)
        mText.setCharacterSize(18u);

    centerOrigin(mText);
}

void GUIButton::select()
{
    GUIObject::select();
    changeAppearance(ButtonState::Selected);
}

void GUIButton::deselect()
{
    GUIObject::deselect();
    changeAppearance(ButtonState::Normal);
}

void GUIButton::deactivate()
{
    GUIObject::deactivate();
    toggle(false);
    changeAppearance(ButtonState::Normal);
}

bool GUIButton::isSelectable() const
{
    return !mIsLocked;
}

void GUIButton::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::MouseButtonReleased:
            runAssignedFunction();
            break;

        case sf::Event::KeyReleased:
            if(event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
            {
                changeAppearance(ButtonState::Pressed);
                mSounds.play(Sound::ButtonClick);
                runAssignedFunction();
            }
            break;
    }
}

sf::FloatRect GUIButton::getBoundingRect() const
{
    switch(mButtonType)
    {
        case ButtonType::Textured:
            return getTransform().transformRect(mSprite.getGlobalBounds());

        case ButtonType::Text:
            return getTransform().transformRect(mText.getGlobalBounds());

        case ButtonType::SimpleRect:
            return getTransform().transformRect(mBox.getGlobalBounds());
    }
}

void GUIButton::setCallback(Callback callback)
{
    mCallback = callback;
}

void GUIButton::toggle(bool isToggled)
{
    mIsToggled = isToggled;
}

void GUIButton::onMouseClick(sf::Vector2i)
{
    if(!mIsLocked)
    {
        activate();
        changeAppearance(ButtonState::Pressed);
        mSounds.play(Sound::ButtonClick);
    }
}

void GUIButton::setRectSize(sf::Vector2f boxSize)
{
    mBox.setSize(boxSize);
    centerText(mBox, mText);
}

void GUIButton::setLocked(bool locked)
{
    mIsLocked = locked;

    if(locked)
        changeAppearance(ButtonState::Locked);
}

void GUIButton::setFreezeFlag(bool freezed)
{
    mFreezeAppearance = freezed;
}

void GUIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mBox, states);
    target.draw(mText, states);
}

void GUIButton::changeAppearance(int state)
{
    if(!mFreezeAppearance)
    {
        switch(mButtonType)
        {
            case ButtonType::Textured:
            {
                sf::IntRect newRect(0, table[mButtonID].buttonSize.y * state, table[mButtonID].buttonSize.x, table[mButtonID].buttonSize.y);
                mSprite.setTextureRect(newRect);
                break;
            }

            case ButtonType::Text:
                if(state == ButtonState::Normal)
                    mText.setFillColor(sf::Color::White);
                else if(state == ButtonState::Locked)
                    mText.setFillColor(sf::Color(114, 114, 114, 100));
                else
                    mText.setFillColor(sf::Color::Red);
                break;

            case ButtonType::SimpleRect:
                switch(state)
                {
                    case ButtonState::Normal:
                        mBox.setFillColor(table[mButtonID].defaultColor);
                        mText.setFillColor(sf::Color::White);
                        break;

                    case ButtonState::Selected:
                        mBox.setFillColor(table[mButtonID].defaultColor);
                        mText.setFillColor(sf::Color::Red);
                        break;

                    case ButtonState::Pressed:
                        mBox.setFillColor(table[mButtonID].activatedColor);
                        mText.setFillColor(sf::Color::Red);
                        break;

                    case ButtonState::Locked:
                        mBox.setFillColor(sf::Color(114, 114, 114, 100));
                        mText.setFillColor(sf::Color(230, 230, 230, 100));
                        break;
                }
                break;
        }
    }
}

void GUIButton::centerButtonOrigin()
{
    switch(mButtonType)
    {
        case GUIButton::Textured:
            centerOrigin(mSprite);
            mText.setPosition(0.f, -mSprite.getLocalBounds().height * 0.1f);
            centerOrigin(mText);
            break;

        case GUIButton::SimpleRect:
            centerOrigin(mBox);
            mText.setPosition(0.f, 0.f);
            centerOrigin(mText);
            break;

        case GUIButton::Text:
            centerOrigin(mText);
            break;
    }
}

void GUIButton::runAssignedFunction()
{
    if(mCallback)
        mCallback();

    if(!mIsToggled)
        deactivate();
}
