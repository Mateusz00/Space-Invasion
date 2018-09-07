#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "KeyBinding.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <map>

class Player : public sf::NonCopyable
{
    public:
        Player();
        void handleRealTimeInput();
        void handleEvent(const sf::Event&);

    private:
};

#endif // PLAYER_HPP
