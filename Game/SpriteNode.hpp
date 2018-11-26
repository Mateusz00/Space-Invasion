#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
    public:
        explicit        SpriteNode(const sf::Texture& texture);
                        SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);

    private:
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

        sf::Sprite mSprite;
};

#endif // SPRITENODE_HPP
