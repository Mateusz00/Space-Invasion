#include "KeyBinding.hpp"
#include "Settings.hpp"

KeyBinding::KeyBinding(int player, Settings& settings)
    : mBinds(settings.getKeyBinding(player)),
      mSettings(settings),
      mPlayer(player)
{
}

void KeyBinding::assignKey(Action action, sf::Keyboard::Key key)
{
    for(const auto& keyActionBind : mBinds)
    {
        if(keyActionBind.second == action)
            mSettings.eraseBind(mPlayer, keyActionBind.first);
            ///mBinds.erase(keyActionBind.first);
    }

    ///mBinds[key] = action;
    mSettings.addBind(mPlayer, key, action);  // Rebinds key to another action if necessary
}

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
    for(const auto& keyToActionBind : mBinds)
    {
        if(keyToActionBind.second == action)
            return keyToActionBind.first;
    }

    return sf::Keyboard::Unknown;
}

bool KeyBinding::findAction(sf::Keyboard::Key key, Action& out) const // Inserts which action pressed key invokes
{
    auto keyToActionBind = mBinds.find(key);
    if(keyToActionBind != mBinds.end())
    {
        out = keyToActionBind->second;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<KeyBinding::Action> KeyBinding::getRealtimeActions() const // Returns all real-time actions that are currently active
{
    std::vector<Action> actions;

    for(const auto& keyToActionBind : mBinds)
    {
        if(sf::Keyboard::isKeyPressed(keyToActionBind.first) && isRealtimeAction(keyToActionBind.second))
            actions.emplace_back(keyToActionBind.second);
    }
    return actions;
}

bool isRealtimeAction(KeyBinding::Action action)
{
    switch (action)
    {
        case KeyBinding::Action::MoveLeft:    return true;
        case KeyBinding::Action::MoveRight:   return true;
        case KeyBinding::Action::MoveDown:    return true;
        case KeyBinding::Action::MoveUp:      return true;
        case KeyBinding::Action::Fire:        return true;
        case KeyBinding::Action::SpeedBoost:  return true;

        default:
            return false;
    }
}
