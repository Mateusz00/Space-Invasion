#include "GUISwitch.hpp"

GUISwitch::GUISwitch(State::Context context, Textures::ID textureId, bool isSwitchedOn)
    : mSprite(context.textures.get(textureId)),
      mIsSwitchedOn(isSwitchedOn)
{
    changeTexture(isSwitchedOn);
}

void GUISwitch::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}

bool GUISwitch::isSelectable() const
{
    return false;
}

void GUISwitch::onMouseClick(sf::Vector2i)
{
    activate();
}

void GUISwitch::setOnCallback(Callback f)
{
    mOnCallback = f;
}

void GUISwitch::setOffCallback(Callback f)
{
    mOffCallback = f;
}

void GUISwitch::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonReleased)
    {
        if(mIsSwitchedOn)
            mOffCallback();
        else
            mOnCallback();

        mIsSwitchedOn = !mIsSwitchedOn;
        changeTexture(mIsSwitchedOn);
        deactivate();
    }
}

sf::FloatRect GUISwitch::getBoundingRect() const
{
    return getTransform().transformRect(mSprite.getGlobalBounds());
}

void GUISwitch::setSwitched(bool flag)
{
    mIsSwitchedOn = flag;
    changeTexture(mIsSwitchedOn);
}

void GUISwitch::changeTexture(bool state) // false - 1 half of texture, true = second
{
    const auto& textureSize = mSprite.getTexture()->getSize();
    mSprite.setTextureRect(sf::IntRect(0.f, (textureSize.y/2.f) * state, textureSize.x, textureSize.y/2.f));
}
