#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "ResourcesID.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <forward_list>

class SoundPlayer
{
    public:
        using DelayedSound = std::pair<sf::Sound, float>;

                SoundPlayer();
        void    play(Sound::ID, float volumeMultiplier = 1.f, float delay = 0.f);
        void    play(Sound::ID, sf::Vector2f, float volumeMultiplier = 1.f, float delay = 0.f);
        void    removeStoppedSounds();
        void    setListener(sf::Vector2f);
        void    setListener(sf::Vector3f);
        void    setVolume(float);
        float   getVolume() const;
        void    mute();
        void    unmute();
        void    update(sf::Time dt);

    private:
        SoundBufferHolder               mSounds;
        std::forward_list<sf::Sound>    mActiveSounds;
        std::vector<DelayedSound>       mSoundsQueue;
        float                           mVolume;
        bool                            mIsMuted;
};

#endif // SOUNDPLAYER_HPP
