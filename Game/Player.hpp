#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "KeyBinding.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include <SFML/Window/Event.hpp>
#include <array>
#include <string>

class Player
{
    public:
        using Action = KeyBinding::Action;

                            Player(KeyBinding*, int id, std::string&);
        void                handleRealTimeInput(CommandQueue&);
        void                handleEvent(const sf::Event&, CommandQueue&);
        void                setScore(int);
        int                 getScore() const;
        int                 getID() const;
        void                setName(const std::string&);
        const std::string&  getName() const;

    private:
        void initializeActions();

        int                                 mIdentifier;
        int                                 mScore;
        KeyBinding*                         mKeyBinding;
        std::string                         mName;
        std::array<Command, Action::Count>  mActionBinding;
};

#endif // PLAYER_HPP
