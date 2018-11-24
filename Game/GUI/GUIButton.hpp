#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include "GUIObject.hpp"
#include "../States/State.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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
			SimpleRect,
			Textured
		};
        enum ButtonID
		{
			ControlsButton,
			TextButton,
			OptionsButton,
			Count
		};
		enum ButtonState
		{
			Normal,
			Selected,
			Pressed
		};

                                GUIButton(State::Context, ButtonID, const std::string&);
        virtual void            select() override;
        virtual void            deselect() override;
        virtual void            deactivate() override;
		virtual bool    		isSelectable() const override;
		virtual void			handleEvent(const sf::Event&) override;
		virtual sf::FloatRect 	getBoundingRect() const override;
		void					setCallback(Callback);
		void                    toggle(bool);
		virtual void            onMouseClick(sf::Vector2i) override;
		void                    setRectSize(sf::Vector2f);
		void                    setFreezeFlag(bool);

	private:
	    virtual void	        draw(sf::RenderTarget&, sf::RenderStates) const override;
		void 					changeAppearance(ButtonState);
		void 					runAssignedFunction();

		ButtonID				mButtonID;
		ButtonType				mButtonType;
	    sf::Sprite      		mSprite;
	    sf::RectangleShape      mBox;
		sf::Text				mText;
		Callback 				mCallback;
		bool					mIsToggled;
		bool					mFreezeAppearance;
		SoundPlayer& 			mSounds;
};

#endif // GUIBUTTON_HPP
