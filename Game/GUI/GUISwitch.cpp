#include "GUISwitch.hpp"

GUISwitch::GUISwitch(State::Context context, Textures::ID textureId)
    : mSprite(context.textures.get(textureId)),
      mIsSwitchedOn(true)
{
    const auto& rect = mSprite.getTextureRect();
    mSprite.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width, rect.height / 2));
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
        changeTexture();
        deactivate();
    }
}

sf::FloatRect GUISwitch::getBoundingRect() const
{
    return getTransform().transformRect(mSprite.getGlobalBounds());
}

void GUISwitch::changeTexture()
{
    const auto& rect = mSprite.getTextureRect();
    unsigned int textureHeight = mSprite.getTexture()->getSize().y;

    mSprite.setTextureRect(sf::IntRect(rect.left, (rect.top + rect.height) % textureHeight, rect.width, rect.height));
}
