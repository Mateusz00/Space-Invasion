#ifndef GUISPEAKER_HPP
#define GUISPEAKER_HPP

#include "GUIObject.hpp"
#include "../States/State.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
/*data[GUIButton::Speaker].textureId = Textures::Speaker;
	data[GUIButton::Speaker].buttonSize = sf::Vector2i(150, 150);*/
class SoundPlayer;
class MusicPlayer;

class GUISpeaker : public GUIObject
{
    public:
                                GUISpeaker(State::Context, SoundPlayer&, MusicPlayer&);
        virtual void	        activate() override;
        virtual void	        deactivate() override;
        virtual void	        handleEvent(const sf::Event&) override;
        virtual sf::FloatRect   getBoundingRect() const override;

    private:
        void    changeTexture();
        void    mute();
        void    unmute();

        sf::Sprite      mSprite;
        SoundPlayer&    mSounds;
        MusicPlayer&    mMusic;
        bool            mIsToggled;
};

#endif // GUISPEAKER_HPP
