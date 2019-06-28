#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "KeyBinding.hpp"
#include <map>
#include <string>
#include <libconfig.h++>

class Settings
{
    public:
        using BindingsArray = std::array<KeyBinding::BindMap, 2>;

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
        ///void            setKeyBinding(KeyBindMap, int);
        const KeyBinding::BindMap&      getKeyBinding(int) const;
        void    eraseBind(int player, sf::Keyboard::Key);
        void    addBind(int player, sf::Keyboard::Key, KeyBinding::Action);

    private:
        void            loadDefaultValues();
        bool            loadFromFile();
        void            addControlsToConfig(libconfig::Setting&, const KeyBinding::BindMap&, const std::string& player);

        bool            mVsync;
        bool            mIsMuted;
        float           mSoundVolume;
        float           mMusicVolume;
        unsigned int    mFramerateLimit;
        BindingsArray   mKeyBindings;
};

#endif // SETTINGS_HPP
