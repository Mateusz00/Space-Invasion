#include "AmmoNode.hpp"
#include "../Utility.hpp"

AmmoNode::AmmoNode(const Spaceship& object, const TextureHolder& textures, const FontHolder& fonts, const sf::View& view)
    : mObject(object),
      mView(view),
      mSprite(textures.get(Textures::Projectiles), Textures::MissileRect)
{
    mAmount.setFont(fonts.get(Fonts::BPmonoItalics));
    mAmount.setCharacterSize(31u);
    mAmount.setString(toString(object.getMissileAmmo()));
}

void AmmoNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    int ammo = mObject.getMissileAmmo();
    if(ammo < 999)
        mAmount.setString(toString(ammo));

    auto viewSize = mView.getSize();
    auto textSpriteSpace = mAmount.getLocalBounds().width + 8.f;

    if(mAlignment == Align::Left)
    {
        mAmount.setPosition(viewSize.x * mMargin, viewSize.y - 50.f);
        mSprite.setPosition(mAmount.getPosition().x + textSpriteSpace, viewSize.y - 45.f);
    }
    else
    {
        mAmount.setPosition(viewSize.x * (1-mMargin) - textSpriteSpace - mSprite.getLocalBounds().width, viewSize.y - 50.f);
        mSprite.setPosition(mAmount.getPosition().x + textSpriteSpace, viewSize.y - 45.f);
    }
}

void AmmoNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mAmount, states);
}

///@param percent - distance in % from border it's aligned to
void AmmoNode::setMargin(float percent)
{
    mMargin = percent / 100.f;
}

void AmmoNode::align(Align a)
{
    mAlignment = a;
}
