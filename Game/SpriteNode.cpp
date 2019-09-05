#include "SpriteNode.hpp"
#include "Utility.hpp"

SpriteNode::SpriteNode(const sf::Texture& texture)
    : mSprite(texture)
{
}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& rect)
    : mSprite(texture, rect)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

sf::FloatRect SpriteNode::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

sf::FloatRect SpriteNode::getLocalBounds() const
{
    return mSprite.getLocalBounds();
}

const sf::IntRect& SpriteNode::getTextureRect() const
{
    return mSprite.getTextureRect();
}

void SpriteNode::setTextureRect(const sf::IntRect& rect) const
{
    mSprite.setTextureRect(rect);
}

const sf::Texture* SpriteNode::getTexture() const
{
    return mSprite.getTexture();
}
