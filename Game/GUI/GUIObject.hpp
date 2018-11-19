#ifndef GUIOBJECT_HPP
#define GUIOBJECT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

class GUIObject : public sf::Drawable, public sf::Transformable
{
    public:
                                GUIObject();
        virtual                 ~GUIObject();
        virtual void            select();
        virtual void            deselect();
        virtual bool            isSelectable() const = 0;
        bool                    isSelected();
        virtual bool            isActive() const;
        virtual void	        activate();
        virtual void	        deactivate();
        virtual void	        handleEvent(const sf::Event&);
        virtual void            onMouseClick(sf::Vector2i);
        virtual sf::FloatRect   getBoundingRect() const;

    private:
        bool mIsSelected;
        bool mIsActive;
};

using ComponentPtr = std::unique_ptr<GUIObject>;

#endif // GUIOBJECT_HPP
