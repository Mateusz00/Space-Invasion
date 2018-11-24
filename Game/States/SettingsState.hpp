#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "State.hpp"
#include "../KeyBinding.hpp"
#include "../Player.hpp"
#include "../GUI/GUIContainer.hpp"
#include "../GUI/GUIButton.hpp"
#include "../GUI/GUILabel.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
#include <string>

class SettingsState : public State
{
    public:
        enum Option
        {
            Controls,
            Others
        };

                        SettingsState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        void	        updateLabels();
        void	        addButton(int index, int player, int y, const std::string&, Context);
        void            addGUIElements(Context);

        sf::Sprite							mBackgroundSprite;
        sf::RectangleShape                  mContainer;
        sf::RectangleShape                  mContainer1;
        GUIContainer                        mOptionButtons;
        GUIContainer                        mControls;
        GUIContainer                        mOtherOptions;
        Option                              mCurrentOption;
        std::pair<bool, Player::Action>     mToggledButton;
        sf::RenderWindow&                   mWindow;
        std::array<GUIButton*, KeyBinding::Count*2>	mBindingButtons;
        std::array<GUILabel*, KeyBinding::Count*2> 	mBindingLabels;
};

#endif // SETTINGSSTATE_HPP
