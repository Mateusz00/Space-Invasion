#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include "../GUI/GUISwitch.hpp"
#include "../Settings.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class MenuState : public State
{
    public:
                        MenuState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        GUIContainer        mGUIContainer;
        sf::RenderWindow&   mWindow;
        sf::Sprite          mBackgroundSprite;
        GUISwitch*          mMute;
        Settings&           mSetttings;
};

#endif // MENUSTATE_HPP
