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

GUIButton::GUIButton(State::Context context, ButtonID id, const std::string& text)
	: mButtonID(id),
	  mButtonType(table[id].buttonType),
	  mText(text, context.fonts.get(Fonts::Sansation), 28u),
	  mIsToggled(false),
	  mFreezeAppearance(false),
	  mSounds(context.sounds)
{
	switch(mButtonType)
	{
        case ButtonType::Textured:
        {
            mSprite.setTexture(context.textures.get(table[mButtonID].textureId));
            changeAppearance(ButtonState::Normal);

            auto bounds = mSprite.getGlobalBounds();
            mText.setPosition(sf::Vector2f(bounds.width  * 0.5f, bounds.height * 0.4f));
            mText.setCharacterSize(18u);
            break;
        }

        case ButtonType::SimpleRect:
        {
            changeAppearance(ButtonState::Normal);
            mText.setCharacterSize(18u);
        }
	}

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
    changeAppearance(ButtonState::Selected);
}

bool GUIButton::isSelectable() const
{
    return true;
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
    activate();
    changeAppearance(ButtonState::Pressed);
    mSounds.play(Sound::ButtonClick);
}

void GUIButton::setRectSize(sf::Vector2f boxSize)
{
    mBox.setSize(boxSize);
    mText.setPosition(sf::Vector2f(boxSize.x  * 0.5f, boxSize.y * 0.4f));
}

void GUIButton::setFreezeFlag(bool flag)
{
    mFreezeAppearance = flag;
}

void GUIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mBox, states);
    target.draw(mText, states);
}

void GUIButton::changeAppearance(ButtonState state)
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
                }
                break;
        }
    }
}

void GUIButton::runAssignedFunction()
{
    if(mCallback)
		mCallback();

	if(!mIsToggled)
		deactivate();
}
