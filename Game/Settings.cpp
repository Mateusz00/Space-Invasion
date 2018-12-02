#include "Settings.hpp"
#include <fstream>
#include <iostream>
using namespace libconfig;

void Settings::load()
{
    // Try to open existing configuration file, use default settings if file not found
    if(!loadFromFile())
        loadDefaultValues();
}

void Settings::saveToFile()
{

}

bool Settings::getVsync() const
{
    return mVsync;
}

void Settings::setVsync(bool flag)
{
    mVsync = flag;
}

void Settings::setMuted(bool flag)
{
    mIsMuted = flag;
}

bool Settings::getMuted() const
{
    return mIsMuted;
}

void Settings::setSoundVolume(float volume)
{
    mSoundVolume = volume;
}

float Settings::getSoundVolume() const
{
    return mSoundVolume;
}

void Settings::setMusicVolume(float volume)
{
    mMusicVolume = volume;
}

float Settings::getMusicVolume() const
{
    return mMusicVolume;
}

void Settings::setFramerateLimit(unsigned int frames)
{
    mFramerateLimit = frames;
}

unsigned int Settings::getFramerateLimit() const
{
    return mFramerateLimit;
}

void Settings::setKeyBinding(KeyBindMap keys, int player)
{
    switch(player)
    {
        case 1:
            mKeyBinding1 = keys;
            break;

        case 2:
            mKeyBinding2 = keys;
            break;
    }
}

Settings::KeyBindMap Settings::getKeyBinding(int player) const
{
    switch(player)
    {
        case 1: return mKeyBinding1;
        case 2: return mKeyBinding2;
    }
}

void Settings::loadDefaultValues()
{
    mVsync          = false;
    mIsMuted        = false;
    mSoundVolume    = 80.f;
    mMusicVolume    = 80.f;
    mFramerateLimit = 0;

    mKeyBinding1[sf::Keyboard::W] = KeyBinding::MoveUp;
    mKeyBinding1[sf::Keyboard::S] = KeyBinding::MoveDown;
    mKeyBinding1[sf::Keyboard::A] = KeyBinding::MoveLeft;
    mKeyBinding1[sf::Keyboard::D] = KeyBinding::MoveRight;
    mKeyBinding1[sf::Keyboard::G] = KeyBinding::Fire;
    mKeyBinding1[sf::Keyboard::H] = KeyBinding::LaunchMissile;

    mKeyBinding2[sf::Keyboard::Up]    = KeyBinding::MoveUp;
    mKeyBinding2[sf::Keyboard::Down]  = KeyBinding::MoveDown;
    mKeyBinding2[sf::Keyboard::Left]  = KeyBinding::MoveLeft;
    mKeyBinding2[sf::Keyboard::Right] = KeyBinding::MoveRight;
    mKeyBinding2[sf::Keyboard::Space] = KeyBinding::Fire;
    mKeyBinding2[sf::Keyboard::M]     = KeyBinding::LaunchMissile;

    saveToFile();
}

bool Settings::loadFromFile()
{
    Config cfg;

    try
    {
        cfg.readFile("config.cfg");
    }
    catch(const FileIOException &fioex)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return false;
    }
    catch(const ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
        return false;
    }

    saveToFile();
    return true;
}
