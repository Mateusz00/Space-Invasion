#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include "GUIObject.hpp"
#include "../States/State.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <functional>
#include <string>
class SoundPlayer;

class GUIButton : public GUIObject
{
    public:
		using Callback = std::function<void()>;

		enum ButtonType
		{
			Text,
			Settings,
			Count
		};
		enum ButtonState
		{
			Normal,
			Selected,
			Pressed
		};

                                GUIButton(State::Context, ButtonType, const std::string&);
        virtual void            select() override;
        virtual void            deselect() override;
        virtual void            activate() override;
        virtual void            deactivate() override;
		virtual bool    		isSelectable() const override;
		virtual void			handleEvent(const sf::Event&) override;
		virtual sf::FloatRect 	getBoundingRect() const override;
		void					setCallback(Callback);

	private:
	    virtual void	        draw(sf::RenderTarget&, sf::RenderStates) const override;
		void 					changeAppearance(ButtonState);

		ButtonType				mType;
	    sf::Sprite      		mSprite;
		sf::Text				mText;
		Callback 				mCallback;
		bool					mIsToggled;
		SoundPlayer& 			mSounds;
};

#endif // GUIBUTTON_HPP
