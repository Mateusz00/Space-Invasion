#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
    public:
        explicit        SpriteNode(const sf::Texture& texture);
                        SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);
        void            center();
        sf::FloatRect   getBoundingRect() const override;

    protected:
        void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        mutable sf::Sprite  mSprite;
};

#endif // SPRITENODE_HPP
