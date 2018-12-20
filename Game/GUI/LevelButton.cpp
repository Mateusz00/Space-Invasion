#include "LevelButton.hpp"

LevelButton::LevelButton(State::Context context, ButtonID id, const std::string& text, int levelID, bool locked)
    : GUIButton(context, id, text, locked)
{
    //ctor
}

void LevelButton::setCompleted()
{
    changeAppearance(Completed);
}
