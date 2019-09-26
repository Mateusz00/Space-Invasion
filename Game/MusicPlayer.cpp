#include "MusicPlayer.hpp"
#include <SFML/Audio/Sound.hpp>

MusicPlayer::MusicPlayer(float volume)
    : mVolume(volume),
      mIsLooped(false),
      mIsMuted(false),
      mCurrentMusicID(Music::None)
{
    mMusicToFilePath[Music::MenuTheme]   = std::string("Resources/MenuTheme.ogg");
    mMusicToFilePath[Music::BattleTheme] = std::string("Resources/BattleTheme.ogg");
}

/// @param checkIfAlreadyPlaying - if set to true, prevents restarting if the requested music is already playing
void MusicPlayer::playNow(Music::ID id, bool loop, bool checkIfAlreadyPlaying)
{
    if(checkIfAlreadyPlaying && mCurrentMusicID == id)
        return;

    mCurrentMusic.stop();
    addToQueue(id, false); // Place it at the beginning of the queue
    setLoop(loop);
}

void MusicPlayer::addToQueue(Music::ID id, bool addToEnd)
{
    if(addToEnd)
        mMusicPlaylist.emplace_back(id);
    else
        mMusicPlaylist.emplace_front(id);
}

void MusicPlayer::update()
{
    if(!mMusicPlaylist.empty() && mCurrentMusic.getStatus() == sf::Sound::Stopped)
    {
        auto id = mMusicPlaylist.front();
        auto filenameIter = mMusicToFilePath.find(id);

        if(filenameIter == mMusicToFilePath.end())
        {
            std::string errorMsg("Could not find file path associated with ID: ");
            errorMsg += toString(static_cast<int>(id));
            throw std::logic_error(errorMsg);
        }

        if(!mCurrentMusic.openFromFile(filenameIter->second))
            throw std::runtime_error("Could not find file: " + filenameIter->second);

        setVolume(mVolume);
        mCurrentMusic.setLoop(mIsLooped);
        mCurrentMusic.play();
        mCurrentMusicID = id;
        mIsLooped = false; // Loop only one sound
        mMusicPlaylist.pop_front(); // Delete from queue as it's already playing
    }
}

void MusicPlayer::setVolume(float volume)
{
    if(mIsMuted)
        mCurrentMusic.setVolume(0.f);
    else
        mCurrentMusic.setVolume(volume);

    mVolume = volume;
}

void MusicPlayer::setLoop(bool isLooped)
{
    mIsLooped = isLooped;
}

float MusicPlayer::getVolume() const
{
    return mVolume;
}

void MusicPlayer::pause()
{
    mCurrentMusic.pause();
}

void MusicPlayer::resume()
{
    mCurrentMusic.play();
}

void MusicPlayer::mute()
{
    mIsMuted = true;
    setVolume(mVolume);
}

void MusicPlayer::unmute()
{
    mIsMuted = false;
    setVolume(mVolume);
}
