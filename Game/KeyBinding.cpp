#include "KeyBinding.hpp"
#include "Settings.hpp"

KeyBinding::KeyBinding(int player, const Settings& settings)
{
    switch(player)
    {
    case 1:
        mBinds = settings.keyBinding1;
        break;

    case 2:
        mBinds = settings.keyBinding2;
        break;
    }

}

void KeyBinding::assignKey(Action action, sf::Keyboard::Key key)
{
    for(auto& keyToActionBind : mBinds)
    {
        if(keyToActionBind.second == action)
            mBinds.erase(keyToActionBind.first);
    }

    mBinds[key] = action; // Rebinds key to another action if necessary
}

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
    for(auto& keyToActionBind : mBinds)
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

    for(auto& keyToActionBind : mBinds)
    {
        if(sf::Keyboard::isKeyPressed(keyToActionBind.first) && isRealtimeAction(keyToActionBind.second))
            actions.push_back(keyToActionBind.second);
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

        default:
            return false;
    }
}
