#include "Settings.hpp"
#include <inicpp/inicpp.h>
#include <fstream>
using namespace inicpp;

void Settings::load()
{
    if(!loadFromFile())
        loadDefaultValues();
}

void Settings::saveToFile(inicpp::config* configPtr)
{
    using namespace inicpp;
    if(configPtr)
    {

    }
    else
    {
        //configPtr->add_section("Settings");
        //configPtr->add_option("Settings", "Vsync", mVsync);
        //parser::save(*configPtr, "config.ini");
    }
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

bool Settings::getSoundVolume() const
{
    return mSoundVolume;
}

void Settings::setMusicVolume(float volume)
{
    mMusicVolume = volume;
}

bool Settings::getMusicVolume() const
{
    return mMusicVolume;
}

void Settings::setFramerateLimit(unsigned int frames)
{
    mFramerateLimit = frames;
}

bool Settings::getFramerateLimit() const
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

    saveToFile(nullptr);
}

bool Settings::loadFromFile()
{
    using namespace inicpp;

    // Try to open existing configuration file, use default settings if file not found
    std::ifstream configHandle("config.ini");
    if(!configHandle.good())
        return false;

    config settings = parser::load(configHandle);

    saveToFile(&settings);
    return true;
}
