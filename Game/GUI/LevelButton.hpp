#ifndef LEVELBUTTON_HPP
#define LEVELBUTTON_HPP

#include "GUIButton.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Sprite.hpp>

class LevelButton : public GUIButton
{
    public:
                        LevelButton(State::Context, ButtonID, const std::string&, int levelID, bool locked = true);
        void            setCompleted();
        virtual void    setLocked(bool) override;

    private:
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override;

        sf::Sprite      mStateIcon;
        TextureHolder&  mTextures;
        bool            mCompleted;
};


#endif // LEVELBUTTON_HPP
