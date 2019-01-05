#ifndef PROFILESTATE_HPP
#define PROFILESTATE_HPP

#include "State.hpp"
#include "../GUI/GUIContainer.hpp"
#include <SFML/Graphics/Sprite.hpp>
class Profile;

class ProfileState : public State
{
    public:
                        ProfileState(Context, StateStack&);
        virtual bool    draw() override;
        virtual bool    update(sf::Time) override;
        virtual bool    handleEvent(const sf::Event&) override;

    private:
        sf::Sprite      mBackground;
        GUIContainer    mOptions;
        Profile&        mProfile;
};

#endif // PROFILESTATE_HPP
