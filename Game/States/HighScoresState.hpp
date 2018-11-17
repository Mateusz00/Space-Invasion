#ifndef HIGHSCORESSTATE_HPP
#define HIGHSCORESSTATE_HPP

#include "State.hpp"
#include "../HighScoresTable.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class HighScoresState : public State
{
    public:
                        HighScoresState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        HighScoresTable     mHighScores;
        sf::Sprite          mBackground;
        sf::Text	        mHeader;
};

#endif // HIGHSCORESSTATE_HPP
