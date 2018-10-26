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

GUIButton::GUIButton(State::Context context, ButtonType type, const std::string& text)
	: mType(type),
	  mText(text, context.fonts.get(Fonts::Sansation), 28u),
	  mSounds(context.sounds)
{
	if(mType != ButtonType::Text)
	{
		mSprite.setTexture(context.textures.get(table[mType].textureId));
		changeAppearance(ButtonState::Normal);
	}

	centerOrigin(mText);
	auto bounds = mSprite.getGlobalBounds();
	mText.setPosition(sf::Vector2f(bounds.width / 2.f, bounds.height / 2.f));
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

void GUIButton::activate()
{
    GUIObject::activate();
	changeAppearance(ButtonState::Pressed);
	mSounds.play(Sound::ButtonClick);

	if(mCallback)
		mCallback();

	if(!mIsToggled)
		deactivate();
}

void GUIButton::deactivate()
{
    GUIObject::deactivate();
	changeAppearance(ButtonState::Selected);
}

bool GUIButton::isSelectable() const
{
    return true;
}

void GUIButton::handleEvent(const sf::Event& event)
{
}

sf::FloatRect GUIButton::getBoundingRect() const
{
    if(mSprite.getTexture())
        return getTransform().transformRect(mSprite.getGlobalBounds());

    return getTransform().transformRect(mText.getGlobalBounds());
}

void GUIButton::setCallback(Callback callback)
{
	mCallback = callback;
}

void GUIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}

void GUIButton::changeAppearance(ButtonState state)
{
	if(mSprite.getTexture())
	{
		sf::IntRect newRect(0, table[mType].buttonSize.x * state, table[mType].buttonSize.x, table[mType].buttonSize.y);
		mSprite.setTextureRect(newRect);
	}
	else
    {
        if(state == ButtonState::Normal)
            mText.setFillColor(sf::Color::White);
        else
            mText.setFillColor(sf::Color::Red);
    }
}
