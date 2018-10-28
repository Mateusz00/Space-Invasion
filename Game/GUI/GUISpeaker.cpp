#include "GUISpeaker.hpp"
#include "../ResourcesID.hpp"
#include "../MusicPlayer.hpp"
#include "../SoundPlayer.hpp"

GUISpeaker::GUISpeaker(State::Context context)
    : mSprite(context.textures.get(Textures::Speaker)),
      mSounds(context.sounds),
      mMusic(context.music),
      mIsToggled(false)
{
    const auto& rect = mSprite.getTextureRect();
    mSprite.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width, rect.height / 2));
}

void GUISpeaker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}

bool GUISpeaker::isSelectable() const
{
    return true;
}

sf::Vector2u GUISpeaker::getSize() const
{
    sf::Vector2f temp(mSprite.getLocalBounds().width, mSprite.getLocalBounds().height);
    return static_cast<sf::Vector2u>(temp);
}

void GUISpeaker::activate()
{
    if(!mIsToggled)
    {
        changeTexture();
        mute();
    }
    else
    {
        changeTexture();
        unmute();
    }
}

void GUISpeaker::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonReleased)
    {
        sf::Vector2i tempPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(tempPos);

        if(getBoundingRect().contains(mousePosition))
            activate();
    }
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

    mSoundLastVolume = mSounds.getVolume();
    mMusicLastVolume = mMusic.getVolume();

    mSounds.setVolume(0.f);
    mMusic.setVolume(0.f);
}

void GUISpeaker::unmute()
{
    mIsToggled = false;

    mSounds.setVolume(mSoundLastVolume);
    mMusic.setVolume(mMusicLastVolume);
}
