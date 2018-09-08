#ifndef KEYBINDING_HPP
#define KEYBINDING_HPP

#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <vector>

class KeyBinding
{
    public:
        enum Action
        {
            MoveUp,
            MoveDown,
            MoveLeft,
            MoveRight,
            Fire,
            LaunchMissile,
            Count
        };

        KeyBinding();
        void assignKey(Action, sf::Keyboard::Key);
        sf::Keyboard::Key getAssignedKey(Action) const; // checks what key is assigned to action(used in SettingsState)
        bool findAction(sf::Keyboard::Key, Action& out) const; // Inserts which action pressed key invokes
        std::vector<Action> getRealtimeActions() const; // Returns all real-time actions that are currently active

    private:
        std::map<sf::Keyboard::Key, Action> mBinds;
};

bool isRealtimeAction(KeyBinding::Action);

#endif // KEYBINDING_HPP
