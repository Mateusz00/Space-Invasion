#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
    public:
        explicit            SpriteNode(const sf::Texture& texture);
                            SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);
        sf::FloatRect       getBoundingRect() const override;
        sf::FloatRect       getLocalBounds() const;
        const sf::IntRect&  getTextureRect() const;
        void                setTextureRect(const sf::IntRect&) const;
        const sf::Texture* 	getTexture() const;

    protected:
        void                drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

    private:
        mutable sf::Sprite  mSprite;
};

#endif // SPRITENODE_HPP
