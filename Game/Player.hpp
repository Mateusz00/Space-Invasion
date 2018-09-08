#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "KeyBinding.hpp"
#include "Command.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <array>

class Player : public sf::NonCopyable
{
    public:
        using Action = KeyBinding::Action;

        Player();
        void handleRealTimeInput();
        void handleEvent(const sf::Event&);

    private:
        std::array<Command, Action::Count> mActionBinding;
};

#endif // PLAYER_HPP
