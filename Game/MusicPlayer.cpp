#include "MusicPlayer.hpp"
#include <SFML/Audio/Sound.hpp>

MusicPlayer::MusicPlayer(float volume = 80.f)
	: mDefaultVolume(volume),
	  mIsLooped(false)
{
}

void MusicPlayer::playNow(Music::ID id)
{
    // Stopped music is deleted from music playlist and music that's first on the list is played instead
	mCurrentMusic.stop();
	addToQueue(id, false);
}

void MusicPlayer::addToQueue(Music::ID id, bool addToEnd = true)
{
	if(addToEnd)
		mMusicPlaylist.emplace_back(id);
	else
		mMusicPlaylist.emplace_front(id);
}

void MusicPlayer::update()
{
	if(mCurrentMusic.getStatus() == sf::Sound::Stopped)
		mMusicPlaylist.pop_front();

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

        mCurrentMusic.setVolume(mDefaultVolume);
        mCurrentMusic.setLoop(mIsLooped);
        mCurrentMusic.play();
        mIsLooped = false; // Loop only one sound
	}
}

void MusicPlayer::setVolume(float volume)
{
    mCurrentMusic.setVolume(volume);
	mDefaultVolume = volume;
}

void MusicPlayer::setLoop(bool isLooped)
{
    mIsLooped = isLooped;
}

float MusicPlayer::getVolume() const
{
	return mDefaultVolume;
}

void MusicPlayer::pause()
{
	mCurrentMusic.pause();
}

void MusicPlayer::resume()
{
	mCurrentMusic.play();
}
