#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameOverState : public State
{
    public:
        enum Type
        {
            Success,
            Fail
        };

                        GameOverState(Context, StateStack&, Type);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        sf::Text            mText;
        sf::Time            mElapsedTime;
        sf::RenderWindow&   mWindow;
        sf::RectangleShape  mBackground;
        GUIContainer        mButtons;
};

#endif // GAMEOVERSTATE_HPP
