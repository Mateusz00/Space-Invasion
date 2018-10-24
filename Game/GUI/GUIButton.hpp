#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include "GUIObject.hpp"
#include <SFML/Graphics.hpp>

class GUIButton : public GUIObject
{
    public:
                                GUIButton();
        virtual void            select() override;
        virtual void            deselect() override;
        virtual void            activate() override;
        virtual void            deactivate() override;
		virtual bool    		isSelectable() const override;
		virtual void			handleEvent(const sf::Event&) override;
		virtual sf::FloatRect 	getBoundingRect() const override;

	private:
	    virtual void	        draw(sf::RenderTarget&, sf::RenderStates) const override;

	    sf::RectangleShape      mSprite;
};

#endif // GUIBUTTON_HPP
