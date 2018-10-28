#ifndef GUISPEAKER_HPP
#define GUISPEAKER_HPP

#include "GUIObject.hpp"
#include "../States/State.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
class SoundPlayer;
class MusicPlayer;

class GUISpeaker : public GUIObject
{
    public:
                        GUISpeaker(State::Context);
        virtual void    handleEvent(const sf::Event&) override;
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override;
        virtual bool    isSelectable() const override;
        sf::Vector2u    getSize() const;

    private:
        virtual void            activate() override;
        virtual sf::FloatRect   getBoundingRect() const override;
        void                    changeTexture();
        void                    mute();
        void                    unmute();

        sf::Sprite      mSprite;
        SoundPlayer&    mSounds;
        MusicPlayer&    mMusic;
        bool            mIsToggled;
        float           mSoundLastVolume;
        float           mMusicLastVolume;
};

#endif // GUISPEAKER_HPP
