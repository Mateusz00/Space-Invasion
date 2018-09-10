#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Category.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <vector>

class SceneNode : public sf::Drawable, public sf::Transformable
{
    public:
        using Ptr = std::unique_ptr<SceneNode>;

                        SceneNode();
        sf::Vector2f    getWorldPosition() const;
        sf::Transform   getWorldTransform() const;
        void            attachChild(Ptr child);
        void            eraseChild(Ptr child);
        void            update(sf::Time);
        virtual Category::Type getCategory() const;

    private:
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override final;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
        void            drawChildren(sf::RenderTarget&, sf::RenderStates) const;
        virtual void    updateCurrent(sf::Time);
        void            updateChildren(sf::Time);

        SceneNode* mParent;
        std::vector<Ptr> mChildren;
};

#endif // SCENENODE_HPP
