
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
	  mText(text, context.fonts.get(Fonts::Sansation), 16u),
	  mSounds(context.sounds)
{
	if(mType != ButtonType::Text)
	{
		mSprite.setTexture(context.textures.get(table[mType].textureId));
		changeTexture(ButtonState::Normal);
	}

	centerOrigin(mText);
	auto bounds = mSprite.getGlobalBounds();
	mText.setPosition(sf::Vector2f(bounds.width / 2.f, bounds.height / 2.f));
}

void GUIButton::select()
{
	GUIObject::select();
	changeTexture(ButtonState::Selected);
}

void GUIButton::deselect()
{
	GUIObject::deselect();
	changeTexture(ButtonState::Normal);
}

void GUIButton::activate()
{
    GUIObject::activate();
	changeTexture(ButtonState::Pressed);
	mSounds.play(Sound::ButtonClick);

	if(mCallback)
		mCallback();

	if(!mIsToggled)
		deactivate();
}

void GUIButton::deactivate()
{
    GUIObject::deactivate();
	changeTexture(ButtonState::Selected);
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
    return getTransform().transformRect(mSprite.getGlobalBounds());
}

void GUIButton::setCallback(Callback callback)
{
	mCallback = callback;
}

void GUIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}

void GUIButton::changeTexture(ButtonState state)
{
	if(mSprite.getTexture())
	{
		sf::IntRect newRect(0, table[mType].rect.top * state, table[mType].rect.width, table[mType].rect.height);
		mSprite.setTextureRect(newRect);
	}
}
