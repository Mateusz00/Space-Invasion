#ifndef PLAYERINFOSTATE_HPP
#define PLAYERINFOSTATE_HPP

#include "State.hpp"
#include "../GUI/GUI_InputBox.hpp"
#include "../GUI/GUIContainer.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

class PlayerInfoState : public State
{
    public:
                        PlayerInfoState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        void            addPlayer(int playerNumber);

        std::vector<Player>&        mPlayers;
        sf::Sprite                  mBackgroundSprite;
        sf::Text                    mText;
        GUI_InputBox                mInputBox;
        sf::RenderWindow&           mWindow;
        std::string                 mPlayerName;
        GUIContainer                mButtons;
        bool                        mModeConfirmed;
        int                         mNumberOfPlayers;
        int                         mCurrentPlayer;
};

#endif // PLAYERINFOSTATE_HPP
