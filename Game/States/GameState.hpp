#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "../World.hpp"
#include "../Player.hpp"

class GameState : public State
{
    public:
        GameState(Context, StateStack*);
        virtual bool draw() override;
        virtual bool update(sf::Time) override;
        virtual bool handleEvent(const sf::Event&) override;

    private:
        World   mWorld;
        Player  mPlayer;
};

#endif // GAMESTATE_HPP
