#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include "ResourcesID.hpp"
#include "Utility.hpp"
#include <SFML/Audio/Music.hpp>
#include <map>
#include <list>
#include <string>
#include <exception>

class MusicPlayer
{
    public:
                MusicPlayer(float volume = 80.f);
        void    playNow(Music::ID, bool); // Stops music and adds new to front of music playlist
        void    addToQueue(Music::ID, bool addToEnd = true);
        void    update(); // Changes stopped music for a new one from front of the playlist and makes it looped depending on the flag
        void    setVolume(float);
        void    setLoop(bool);
        float   getVolume() const;
        void    pause();
        void    resume();
        void    mute();
        void    unmute();

    private:
        std::map<Music::ID, std::string>    mMusicToFilePath;
        std::list<Music::ID>                mMusicPlaylist;
        float                               mVolume;
        sf::Music                           mCurrentMusic;
        bool                                mIsLooped; // Indicates if next music should be looped
        float                               mVolumeBeforeMuting;
};

#endif // MUSICPLAYER_HPP
