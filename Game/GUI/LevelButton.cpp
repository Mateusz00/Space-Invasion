#include "LevelButton.hpp"
#include "../Utility.hpp"

LevelButton::LevelButton(State::Context context, ButtonID id, const std::string& text, int levelID, bool locked)
    : GUIButton(context, id, text, locked),
      mTextures(context.textures),
      mCompleted(false)
{
    mStateIcon.setPosition(sf::Vector2f(0.f, 0.f));
    setLocked(locked);
}

void LevelButton::setCompleted()
{
    mCompleted = true;
    mStateIcon.setTexture(mTextures.get(Textures::Checkmark), true);
    centerOrigin(mStateIcon);
}

void LevelButton::setLocked(bool locked)
{
    GUIButton::setLocked(locked);

    if(locked)
    {
        mStateIcon.setTexture(mTextures.get(Textures::Lock), true);
        centerOrigin(mStateIcon);
    }
    else
    {
        if(!mCompleted)
            mStateIcon.setTextureRect(sf::IntRect(0,0,0,0));

        changeAppearance(Normal);
    }
}

void LevelButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GUIButton::draw(target, states);

    states.transform *= getTransform();
    target.draw(mStateIcon, states);
}
