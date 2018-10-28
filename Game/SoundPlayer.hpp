#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "ResourcesID.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Vector2.hpp>
#include <forward_list>

class SoundPlayer
{
	public:
                SoundPlayer();
		void    play(Sound::ID);
		void    play(Sound::ID, sf::Vector2f);
		void    removeStoppedSounds();
		void    setListener(sf::Vector2f);
		void    setVolume(float);
		float   getVolume() const;

	private:
		SoundBufferHolder				mSounds;
		std::forward_list<sf::Sound>	mActiveSounds;
		float 							mDefaultVolume;
};

#endif // SOUNDPLAYER_HPP
