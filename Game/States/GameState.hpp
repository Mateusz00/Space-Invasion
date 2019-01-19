#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "../World.hpp"
#include "../Player.hpp"
#include "../SoundPlayer.hpp"
class Profile;

class GameState : public State
{
    public:
                        GameState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        void            updateScoresFile() const;
        void            updatePlayersScore();
        int             getCurrentCumulativeScore();

        World                   mWorld;
        std::vector<Player>&    mPlayers;
        sf::RenderWindow&       mWindow;
        Profile&                mProfile;
};

#endif // GAMESTATE_HPP
