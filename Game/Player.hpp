#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "KeyBinding.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include <SFML/Window/Event.hpp>
#include <array>

class Player
{
    public:
        using Action = KeyBinding::Action;

             Player(KeyBinding*, int id);
        void handleRealTimeInput(CommandQueue&);
        void handleEvent(const sf::Event&, CommandQueue&);
        void setScore(int);
        int  getScore() const;
        int  getID() const;

    private:
        void initializeActions();

        int         mIdentifier;
        int         mScore;
        KeyBinding* mKeyBinding;
        std::array<Command, Action::Count> mActionBinding;
};

#endif // PLAYER_HPP
