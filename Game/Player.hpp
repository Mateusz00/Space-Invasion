#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "KeyBinding.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <array>

class Player : public sf::NonCopyable
{
    public:
        using Action = KeyBinding::Action;

        Player(KeyBinding*, int id);
        void handleRealTimeInput(CommandQueue&);
        void handleEvent(const sf::Event&, CommandQueue&);

    private:
        std::array<Command, Action::Count> mActionBinding;
        KeyBinding* mKeyBinding;
        int mIdentifier;
};

#endif // PLAYER_HPP
