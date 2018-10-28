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
#include <array>
#include <string>

class SettingsState : public State
{
    public:
                        SettingsState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        void	updateLabels();
        void	addButton(int index, int y, const std::string&, Context);

        sf::Sprite								    mBackgroundSprite;
        GUIContainer								mGUIContainer;
        std::array<GUIButton*, KeyBinding::Count>	mBindingButtons;
        std::array<GUILabel*, KeyBinding::Count> 	mBindingLabels;
        std::pair<bool, Player::Action>             mToggledButton;
        sf::RenderWindow&                           mWindow;
};

#endif // SETTINGSSTATE_HPP
