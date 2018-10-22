#ifndef GUIOBJECT_HPP
#define GUIOBJECT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

class GUIObject : public sf::Drawable, public sf::Transformable
{
    public:
        using ComponentPtr = std::unique_ptr<GUIObject>;

                                GUIObject();
        virtual                 ~GUIObject();
        virtual void            select();
        virtual void            deselect();
        virtual bool            isSelectable() const = 0;
        bool                    isSelected();
        virtual bool            isActive() const;
        virtual void	        activate();
        virtual void	        deactivate();
        virtual void	        handleEvent(const sf::Event& event) = 0;
        virtual sf::FloatRect   getBoundingRect() const = 0;

    private:
        bool mIsSelected;
        bool mIsActive;
};

#endif // GUIOBJECT_HPP
