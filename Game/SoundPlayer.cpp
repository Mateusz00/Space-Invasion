#include "SoundPlayer.hpp"
#include <cmath>
#include <SFML/System/Vector3.hpp>
#include <SFML/Audio/Listener.hpp>
namespace
{
    const float ATTENUATION = 8.f;
    const float LISTENER_Z = 500.f;
    const float MIN_DISTANCE_2D = 120.f;
    const float MIN_DISTANCE_3D = std::sqrt(MIN_DISTANCE_2D * MIN_DISTANCE_2D + LISTENER_Z * LISTENER_Z);
}

SoundPlayer::SoundPlayer()
    : mVolume(80.f),
      mIsMuted(false)
{
    mSounds.loadFromFile(Sound::AllyGun,        "Resources/GunShot2.ogg");
    mSounds.loadFromFile(Sound::EnemyGun,       "Resources/GunShot.ogg");
    mSounds.loadFromFile(Sound::Explosion,      "Resources/Explosion.ogg");
    mSounds.loadFromFile(Sound::GameOver,       "Resources/GameOver.ogg");
    mSounds.loadFromFile(Sound::Missile,        "Resources/MissileLaunch.ogg");
    mSounds.loadFromFile(Sound::PickUp,         "Resources/PickUp.ogg");
    mSounds.loadFromFile(Sound::ButtonClick,    "Resources/ButtonClick.ogg");
}

void SoundPlayer::play(Sound::ID id, float volumeMultiplier)
{
    play(id, sf::Vector2f(sf::Listener::getPosition().x, sf::Listener::getPosition().y), volumeMultiplier);
}

void SoundPlayer::play(Sound::ID id, sf::Vector2f position, float volumeMultiplier)
{
    mActiveSounds.push_front(sf::Sound());
    sf::Sound& sound = mActiveSounds.front();

    sound.setBuffer(mSounds.get(id));
    sound.setPosition(position.x, position.y, 0.f);
    sound.setMinDistance(MIN_DISTANCE_3D);
    sound.setAttenuation(ATTENUATION);

    if(mIsMuted)
        sound.setVolume(0.f);
    else
        sound.setVolume(mVolume * volumeMultiplier);

    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    mActiveSounds.remove_if([](sf::Sound& x)
    {
        return x.getStatus() == sf::Sound::Stopped;
    });
}

void SoundPlayer::setListener(sf::Vector2f position)
{
    setListener(sf::Vector3f(position.x, position.y, LISTENER_Z));
}

void SoundPlayer::setListener(sf::Vector3f position)
{
    sf::Listener::setPosition(position);
}

void SoundPlayer::setVolume(float volume)
{
    mVolume = volume;
}

float SoundPlayer::getVolume() const
{
    return mVolume;
}

void SoundPlayer::mute()
{
    mIsMuted = true;
}

void SoundPlayer::unmute()
{
    mIsMuted = false;
}
