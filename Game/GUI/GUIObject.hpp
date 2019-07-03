#ifndef GUIOBJECT_HPP
#define GUIOBJECT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <memory>

class GUIObject : public sf::Drawable, public sf::Transformable
{
    public:
                                GUIObject();
        virtual                 ~GUIObject();
        virtual void            select();
        virtual void            deselect();
        virtual bool            isSelectable() const = 0;
        virtual void            update(sf::Time);
        bool                    isSelected();
        virtual bool            isActive() const;
        void                    activate();
        virtual void            deactivate();
        virtual void            handleEvent(const sf::Event&);
        virtual void            onMouseClick(sf::Vector2i);
        virtual sf::FloatRect   getBoundingRect() const;
        void                    setAbsolutePosition();
        bool                    hasAbsolutePosition() const;

    private:
        bool    mIsSelected;
        bool    mIsActive;
        bool    mHasAbsolutePosition;
};

using ComponentPtr = std::unique_ptr<GUIObject>;

#endif // GUIOBJECT_HPP
