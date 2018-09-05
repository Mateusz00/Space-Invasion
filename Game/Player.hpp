#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Window/Event.hpp>

class Player
{
    public:
        Player();
        void handleEvent(const sf::Event& event);
        void handleRealTimeInput();

    private:
};

#endif // PLAYER_HPP
