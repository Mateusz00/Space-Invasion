#include "GUISpeaker.hpp"
#include "../ResourcesID.hpp"

GUISpeaker::GUISpeaker(State::Context context, SoundPlayer& sounds, MusicPlayer& music)
    : mSprite(context.textures.get(Textures::Speaker)),
      mSounds(sounds),
      mMusic(music),
      mIsToggled(false)
{
    const auto& rect = mSprite.getTextureRect();
    mSprite.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width / 2, rect.height / 2));
}

void GUISpeaker::activate()
{
    if(!mIsToggled) // Avoid muting already muted sound
    {
        GUIObject::activate();
        changeTexture();
        mute();
    }
}

void GUISpeaker::deactivate()
{
    if(mIsToggled) // Avoid unmuting already unmuted sound
    {
        GUIObject::deactivate();
        changeTexture();
        unmute();
    }
}

void GUISpeaker::handleEvent(const sf::Event&)
{
}

sf::FloatRect GUISpeaker::getBoundingRect() const
{
    return getTransform().transformRect(mSprite.getGlobalBounds());
}

void GUISpeaker::changeTexture()
{
    const auto& rect = mSprite.getTextureRect();
    unsigned int textureHeight = mSprite.getTexture()->getSize().y;

    mSprite.setTextureRect(sf::IntRect(rect.left, (rect.top + rect.height) % textureHeight, rect.width, rect.height));
}

void GUISpeaker::mute()
{
    mIsToggled = true;
}

void GUISpeaker::unmute()
{
    mIsToggled = false;
}
