#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "KeyBinding.hpp"
#include <map>

struct Settings
{
    void    loadDefaultValues();
    void    loadFromFile();

    bool            vsync;
    bool            isMuted;
    float           soundVolume;
    float           musicVolume;
    bool            limitFramerate;
    unsigned int    frameLimit;
    std::map<sf::Keyboard::Key, KeyBinding::Action>     keyBinding1;
    std::map<sf::Keyboard::Key, KeyBinding::Action>     keyBinding2;
};

#endif // SETTINGS_HPP
