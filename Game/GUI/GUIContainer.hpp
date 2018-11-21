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
                                GUIContainer(bool allowKeyboardNavigation = true, bool mVerticalNavigation = true);
        void			        push(ComponentPtr);
		virtual bool    		isSelectable() const override;
		virtual void			handleEvent(const sf::Event&) override;
		virtual sf::FloatRect 	getBoundingRect() const override;
		void                    setVerticalNavigation(bool); // False means user can navigate container horizontally
		void                    useKeyboardNavigation(bool);

	private:
	    virtual void	        draw(sf::RenderTarget&, sf::RenderStates) const override;
        void		            selectNext();
        void			        selectPrevious();
        void			        deselect();
        bool                    hasSelection() const;
		sf::FloatRect           getComponentRect(int componentNumber) const;
		bool                    checkMouseCollision(sf::Vector2i, int& index) const;
		void                    restart(); // Removes selection and deactivates selected object

		std::vector<ComponentPtr>	mComponents;
		int                         mSelected;
		bool                        mAllowKeyboardNavigation;
		bool                        mVerticalNavigation;
};

#endif // GUICONTAINER_HPP
