#include "AmmoNode.hpp"
#include "../Utility.hpp"

AmmoNode::AmmoNode(const Aircraft& object, const TextureHolder& textures, const FontHolder& fonts, const sf::View& view)
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
    auto amountBounds = mAmount.getLocalBounds();
    mAmount.setPosition(viewSize.x - amountBounds.width - 40.f, viewSize.y - 50.f);
    mSprite.setPosition(viewSize.x - 30.f, viewSize.y - 45.f);
}

void AmmoNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mAmount, states);
}
