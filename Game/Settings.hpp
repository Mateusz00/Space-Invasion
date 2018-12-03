#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "KeyBinding.hpp"
#include <map>
#include <string>
#include <libconfig.h++>

class Settings
{
    public:
        using KeyBindMap = std::map<sf::Keyboard::Key, KeyBinding::Action>;

        void            load();
        void            saveToFile();
        bool            getVsync() const;
        void            setVsync(bool);
        void            setMuted(bool);
        bool            getMuted() const;
        void            setSoundVolume(float);
        float           getSoundVolume() const;
        void            setMusicVolume(float);
        float           getMusicVolume() const;
        void            setFramerateLimit(unsigned int);
        unsigned int    getFramerateLimit() const;
        void            setKeyBinding(KeyBindMap, int);
        KeyBindMap      getKeyBinding(int) const;

    private:
        void            loadDefaultValues();
        bool            loadFromFile();
        void            addControlsToConfig(libconfig::Setting&, const KeyBindMap&, const std::string& player);

        bool            mVsync;
        bool            mIsMuted;
        float           mSoundVolume;
        float           mMusicVolume;
        unsigned int    mFramerateLimit;
        KeyBindMap      mKeyBinding1;
        KeyBindMap      mKeyBinding2;
};

#endif // SETTINGS_HPP
