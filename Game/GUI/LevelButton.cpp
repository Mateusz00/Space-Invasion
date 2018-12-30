#include "LevelButton.hpp"
#include "../Utility.hpp"

LevelButton::LevelButton(State::Context context, ButtonID id, const std::string& text, int levelID, bool locked)
    : GUIButton(context, id, text, locked),
      mTextures(context.textures)
{
    mStateIcon.setPosition(sf::Vector2f(25.f, 25.f));
    setLocked(locked);
}

void LevelButton::setCompleted()
{
    changeAppearance(Completed);
    mStateIcon.setTexture(mTextures.get(Textures::Checkmark));
    centerOrigin(mStateIcon);
}

void LevelButton::setLocked(bool locked)
{
    GUIButton::setLocked(locked);

    if(locked)
    {
        mStateIcon.setTexture(mTextures.get(Textures::Lock));
        centerOrigin(mStateIcon);
    }
}

void LevelButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GUIButton::draw(target, states);

    states.transform *= getTransform();
    target.draw(mStateIcon, states);
}
