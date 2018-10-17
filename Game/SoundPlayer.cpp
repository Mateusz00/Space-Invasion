#include "SoundPlayer.hpp"
#include <cmath>
#include <SFML/System/Vector3.hpp>
#include <SFML/Audio/Listener.hpp>
namespace
{
	const float ATTENUATION = 10.f;
	const float LISTENER_Z = 100.f;
	const float MIN_DISTANCE_2D = 200.f;
	const float MIN_DISTANCE_3D = std::sqrt(MIN_DISTANCE_2D * MIN_DISTANCE_2D + LISTENER_Z * LISTENER_Z);
}

SoundPlayer::SoundPlayer()
{
	// Load sounds to mSounds
}

void SoundPlayer::play(Sound::ID id)
{
	play(id, sf::Vector2f(0.f, 0.f));
}

void SoundPlayer::play(Sound::ID id, sf::Vector2f position)
{
	mActiveSounds.push_front(sf::Sound());
	sf::Sound& sound = mActiveSounds.front();

	sound.setBuffer(mSounds.get(id));
	sound.setPosition(position.x, position.y, 0.f);
	sound.setMinDistance(MIN_DISTANCE_3D);
	sound.setAttenuation(ATTENUATION);
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
	sf::Listener::setPosition(sf::Vector3f(position.x, position.y, LISTENER_Z));
}
