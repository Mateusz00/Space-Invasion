#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include <SFML/Graphics/Text.hpp>

class PauseState : public State
{
    public:
                        PauseState(Context, StateStack&, bool isNetworked);
        virtual         ~PauseState() override;
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        sf::RenderWindow&   mWindow;
        bool                mIsNetworked;
        sf::Text            mText;
        GUIContainer        mGUIContainer;
};

#endif // PAUSESTATE_HPP
