#include "Settings.hpp"

void Settings::loadDefaultValues()
{
    vsync = false;
    isMuted = false;
    soundVolume = 80.f;
    musicVolume = 80.f;
    limitFramerate = false;
    frameLimit = 0;

    keyBinding1[sf::Keyboard::W] = KeyBinding::MoveUp;
    keyBinding1[sf::Keyboard::S] = KeyBinding::MoveDown;
    keyBinding1[sf::Keyboard::A] = KeyBinding::MoveLeft;
    keyBinding1[sf::Keyboard::D] = KeyBinding::MoveRight;
    keyBinding1[sf::Keyboard::G] = KeyBinding::Fire;
    keyBinding1[sf::Keyboard::H] = KeyBinding::LaunchMissile;

    keyBinding2[sf::Keyboard::Up]    = KeyBinding::MoveUp;
    keyBinding2[sf::Keyboard::Down]  = KeyBinding::MoveDown;
    keyBinding2[sf::Keyboard::Left]  = KeyBinding::MoveLeft;
    keyBinding2[sf::Keyboard::Right] = KeyBinding::MoveRight;
    keyBinding2[sf::Keyboard::Space] = KeyBinding::Fire;
    keyBinding2[sf::Keyboard::M]     = KeyBinding::LaunchMissile;
}

void Settings::loadFromFile()
{
    /// TODO: Write this function, use ini files(simpleini) or xml
}
