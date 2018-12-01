#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "KeyBinding.hpp"
#include <inicpp/config.h>
#include <map>

class Settings
{
    public:
        using KeyBindMap = std::map<sf::Keyboard::Key, KeyBinding::Action>;

        void        load();
        void        saveToFile(inicpp::config*);
        bool        getVsync() const;
        void        setVsync(bool);
        void        setMuted(bool);
        bool        getMuted() const;
        void        setSoundVolume(float);
        bool        getSoundVolume() const;
        void        setMusicVolume(float);
        bool        getMusicVolume() const;
        void        setFramerateLimit(unsigned int);
        bool        getFramerateLimit() const;
        void        setKeyBinding(KeyBindMap, int);
        KeyBindMap  getKeyBinding(int) const;

    private:
        void        loadDefaultValues();
        bool        loadFromFile();

        bool            mVsync;
        bool            mIsMuted;
        float           mSoundVolume;
        float           mMusicVolume;
        unsigned int    mFramerateLimit;
        KeyBindMap      mKeyBinding1;
        KeyBindMap      mKeyBinding2;
};

#endif // SETTINGS_HPP
