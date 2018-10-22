#ifndef GUICONTAINER_HPP
#define GUICONTAINER_HPP

#include "GUIObject.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

class GUIContainer : public GUIObject
{
	public:
                                GUIContainer();
        void			        push(ComponentPtr);
		virtual bool    		isSelectable() const override;
		virtual void			handleEvent(const sf::Event&) override;
		virtual sf::FloatRect 	getBoundingRect() const override;
		void                    update(sf::Window&);

	private:
	    virtual void	        draw(sf::RenderTarget&, sf::RenderStates) const override;
        void		            selectNext();
        void			        selectPrevious();
        bool                    hasSelection() const;
		GUIObject*				checkForToggledButtons() const;
		sf::FloatRect           getComponentRect(int componentNumber) const;

		std::vector<ComponentPtr>	mComponents;
		int                         mSelected;
};

#endif // GUICONTAINER_HPP
