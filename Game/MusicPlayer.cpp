#include "MusicPlayer.hpp"
#include <SFML/Audio/Sound.hpp>

MusicPlayer::MusicPlayer(float volume)
	: mVolume(volume),
	  mIsLooped(false)
{
    mMusicToFilePath[Music::MenuTheme]   = std::string("Resources/MenuTheme.ogg");
	mMusicToFilePath[Music::BattleTheme] = std::string("Resources/BattleTheme.ogg");
}

void MusicPlayer::playNow(Music::ID id, bool isLooped)
{
	mCurrentMusic.stop();
	addToQueue(id, false); // Place it at the beginning of the queue
	setLoop(isLooped);
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

        mCurrentMusic.setVolume(mVolume);
        mCurrentMusic.setLoop(mIsLooped);
        mCurrentMusic.play();
        mIsLooped = false; // Loop only one sound
        mMusicPlaylist.pop_front(); // Delete from queue as it's already playing
	}
}

void MusicPlayer::setVolume(float volume)
{
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
    mVolumeBeforeMuting = mVolume;
    setVolume(0.f);
}

void MusicPlayer::unmute()
{
    setVolume(mVolumeBeforeMuting);
}
