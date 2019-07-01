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

        for(int i=0; i < mKeyBindings.size(); ++i)
            addControlsToConfig(root, mKeyBindings[i], "Player" + toString(i+1));
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

const KeyBinding::BindMap& Settings::getKeyBinding(int player) const
{
    return mKeyBindings.at(player);
}

void Settings::eraseBind(int player, sf::Keyboard::Key key)
{
    mKeyBindings.at(player).erase(key);
}

void Settings::addBind(int player, sf::Keyboard::Key key, KeyBinding::Action action)
{
    (mKeyBindings.at(player))[key] = action;
}

void Settings::loadDefaultValues()
{
    mVsync          = false;
    mIsMuted        = false;
    mSoundVolume    = 80.f;
    mMusicVolume    = 80.f;
    mFramerateLimit = 0;

    mKeyBindings[0][sf::Keyboard::W] = KeyBinding::MoveUp;
    mKeyBindings[0][sf::Keyboard::S] = KeyBinding::MoveDown;
    mKeyBindings[0][sf::Keyboard::A] = KeyBinding::MoveLeft;
    mKeyBindings[0][sf::Keyboard::D] = KeyBinding::MoveRight;
    mKeyBindings[0][sf::Keyboard::F] = KeyBinding::Fire;
    mKeyBindings[0][sf::Keyboard::G] = KeyBinding::LaunchMissile;
    mKeyBindings[0][sf::Keyboard::H] = KeyBinding::SpeedBoost;

    mKeyBindings[1][sf::Keyboard::Up]       = KeyBinding::MoveUp;
    mKeyBindings[1][sf::Keyboard::Down]     = KeyBinding::MoveDown;
    mKeyBindings[1][sf::Keyboard::Left]     = KeyBinding::MoveLeft;
    mKeyBindings[1][sf::Keyboard::Right]    = KeyBinding::MoveRight;
    mKeyBindings[1][sf::Keyboard::Comma]    = KeyBinding::Fire;
    mKeyBindings[1][sf::Keyboard::Period]   = KeyBinding::LaunchMissile;
    mKeyBindings[1][sf::Keyboard::Slash]    = KeyBinding::SpeedBoost;

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

        for(int i=0; i < mKeyBindings.size(); ++i)
        {
            std::string playerNumber = toString(i+1);

            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_MoveUp"))]          = KeyBinding::MoveUp;
            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_MoveDown"))]        = KeyBinding::MoveDown;
            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_MoveLeft"))]        = KeyBinding::MoveLeft;
            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_MoveRight"))]       = KeyBinding::MoveRight;
            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_Fire"))]            = KeyBinding::Fire;
            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_LaunchMissile"))]   = KeyBinding::LaunchMissile;
            mKeyBindings[i][toKey(cfg.lookup("Player" + playerNumber + "_SpeedBoost"))]      = KeyBinding::SpeedBoost;
        }
    }
    catch(const SettingNotFoundException& settingEx)
    {
        std::cerr << "setting not found: " << settingEx.getPath() << std::endl;
        for(auto& bindMap : mKeyBindings)
            bindMap.clear();

        return false;
    }
    catch(const SettingTypeException& typeEx)
    {
        std::cerr << "Type of a setting’s value does not match the type requested: " << typeEx.getPath() << std::endl;
        for(auto& bindMap : mKeyBindings)
            bindMap.clear();

        return false;
    }

    return true;
}

void Settings::addControlsToConfig(libconfig::Setting& root, const KeyBinding::BindMap& keyBindings, const std::string& player)
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
