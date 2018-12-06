#ifndef GUISWITCH_HPP
#define GUISWITCH_HPP

#include "GUIObject.hpp"
#include "../States/State.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class GUISwitch : public GUIObject
{
    public:
        using Callback = std::function<void()>;

                                GUISwitch(State::Context, Textures::ID, bool isSwitchedOn);
        virtual void            handleEvent(const sf::Event&) override;
        virtual void            draw(sf::RenderTarget&, sf::RenderStates) const override;
        virtual bool            isSelectable() const override;
        virtual void            onMouseClick(sf::Vector2i) override;
        void                    setOnCallback(Callback);
        void                    setOffCallback(Callback);
        virtual sf::FloatRect   getBoundingRect() const override;
        void                    setSwitched(bool);

    private:
        void                    changeTexture(bool);

        sf::Sprite      mSprite;
        bool            mIsSwitchedOn;
        Callback        mOffCallback;
        Callback        mOnCallback;
};

#endif // GUISWITCH_HPP
