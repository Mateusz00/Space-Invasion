#include "Settings.hpp"
#include "Utility.hpp"
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
    Config cfg;

    Setting& root = cfg.getRoot();
    try
    {
        root.add("Vsync", Setting::TypeBoolean) = mVsync;
        root.add("Mute", Setting::TypeBoolean) = mIsMuted;
        root.add("MusicVolume", Setting::TypeFloat) = mMusicVolume;
        root.add("SoundVolume", Setting::TypeFloat) = mSoundVolume;
        root.add("FrameLimit", Setting::TypeInt) = static_cast<int>(mFramerateLimit);
        addControlsToConfig(root, mKeyBinding1, "Player1");
        addControlsToConfig(root, mKeyBinding2, "Player2");
    }
    catch(const SettingTypeException& typeEx)
    {
        std::cerr << "Wrong data type assigned: " << typeEx.getPath() << std::endl;
    }
    catch(const SettingNameException& nameEx)
    {
        std::cerr << "Setting has a non-unique or invalid name: " << nameEx.getPath() << std::endl;
    }

    try
    {
        cfg.writeFile("config.cfg");
        std::cout << "Updated configuration successfully written" << std::endl;
    }
    catch(const FileIOException &fioEx)
    {
        std::cerr << "I/O error while writing file" << std::endl;
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
    mKeyBinding1[sf::Keyboard::F] = KeyBinding::Fire;
    mKeyBinding1[sf::Keyboard::G] = KeyBinding::LaunchMissile;
    mKeyBinding1[sf::Keyboard::H] = KeyBinding::SpeedBoost;

    mKeyBinding2[sf::Keyboard::Up]          = KeyBinding::MoveUp;
    mKeyBinding2[sf::Keyboard::Down]        = KeyBinding::MoveDown;
    mKeyBinding2[sf::Keyboard::Left]        = KeyBinding::MoveLeft;
    mKeyBinding2[sf::Keyboard::Right]       = KeyBinding::MoveRight;
    mKeyBinding2[sf::Keyboard::Comma]       = KeyBinding::Fire;
    mKeyBinding2[sf::Keyboard::Period]      = KeyBinding::LaunchMissile;
    mKeyBinding2[sf::Keyboard::Slash]       = KeyBinding::SpeedBoost;

    saveToFile();
}

bool Settings::loadFromFile()
{
    Config cfg;
    try
    {
        cfg.readFile("config.cfg");
    }
    catch(const FileIOException &fioEx)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return false;
    }
    catch(const ParseException &pEx)
    {
        std::cerr << "Parse error at " << pEx.getFile() << ":" << pEx.getLine() << " - " << pEx.getError() << std::endl;
        return false;
    }

    try
    {
        mVsync          = cfg.lookup("Vsync");
        mIsMuted        = cfg.lookup("Mute");
        mMusicVolume    = cfg.lookup("MusicVolume");
        mSoundVolume    = cfg.lookup("SoundVolume");
        mFramerateLimit = cfg.lookup("FrameLimit");

        mKeyBinding1[toKey(cfg.lookup("Player1_MoveUp"))]          = KeyBinding::MoveUp;
        mKeyBinding1[toKey(cfg.lookup("Player1_MoveDown"))]        = KeyBinding::MoveDown;
        mKeyBinding1[toKey(cfg.lookup("Player1_MoveLeft"))]        = KeyBinding::MoveLeft;
        mKeyBinding1[toKey(cfg.lookup("Player1_MoveRight"))]       = KeyBinding::MoveRight;
        mKeyBinding1[toKey(cfg.lookup("Player1_Fire"))]            = KeyBinding::Fire;
        mKeyBinding1[toKey(cfg.lookup("Player1_LaunchMissile"))]   = KeyBinding::LaunchMissile;
        mKeyBinding1[toKey(cfg.lookup("Player1_SpeedBoost"))]      = KeyBinding::SpeedBoost;

        mKeyBinding2[toKey(cfg.lookup("Player2_MoveUp"))]          = KeyBinding::MoveUp;
        mKeyBinding2[toKey(cfg.lookup("Player2_MoveDown"))]        = KeyBinding::MoveDown;
        mKeyBinding2[toKey(cfg.lookup("Player2_MoveLeft"))]        = KeyBinding::MoveLeft;
        mKeyBinding2[toKey(cfg.lookup("Player2_MoveRight"))]       = KeyBinding::MoveRight;
        mKeyBinding2[toKey(cfg.lookup("Player2_Fire"))]            = KeyBinding::Fire;
        mKeyBinding2[toKey(cfg.lookup("Player2_LaunchMissile"))]   = KeyBinding::LaunchMissile;
        mKeyBinding2[toKey(cfg.lookup("Player2_SpeedBoost"))]      = KeyBinding::SpeedBoost;
    }
    catch(const SettingNotFoundException& settingEx)
    {
        std::cerr << "setting not found: " << settingEx.getPath() << std::endl;
        mKeyBinding1.clear();
        mKeyBinding2.clear();
        return false;
    }
    catch(const SettingTypeException& typeEx)
    {
        std::cerr << "Type of a setting’s value does not match the type requested: " << typeEx.getPath() << std::endl;
        mKeyBinding1.clear();
        mKeyBinding2.clear();
        return false;
    }

    return true;
}

void Settings::addControlsToConfig(libconfig::Setting& root, const KeyBindMap& keyBindings, const std::string& player)
{
    #define ADD_ACTION_CASE(ACTION) \
    case KeyBinding::Action::ACTION:  root.add(player + "_"#ACTION, Setting::TypeString) = toString(keyBind.first);  break;

    for(const auto& keyBind : keyBindings)
    {
        switch(keyBind.second)
        {
            ADD_ACTION_CASE(MoveUp)
            ADD_ACTION_CASE(MoveDown)
            ADD_ACTION_CASE(MoveLeft)
            ADD_ACTION_CASE(MoveRight)
            ADD_ACTION_CASE(Fire)
            ADD_ACTION_CASE(LaunchMissile)
            ADD_ACTION_CASE(SpeedBoost)
        }
    }
}
