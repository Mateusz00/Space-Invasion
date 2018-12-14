#include "LevelState.hpp"
const int LevelState::mLevelNumber = 6; // TODO: If possible make number of buttons dynamic using XML

LevelState::LevelState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mBackground(context.textures.get(Textures::TitleScreen)),
      mButtons(true, false),
      mLines(sf::Triangles, mLevelNumber*2),
      mLevel(0) //temp solution
{
    for(int i=0; i < mLevelNumber; ++i)
        createLevelButton(i);

    createConnectionLines();
}

bool LevelState::draw()
{
    auto& window = getContext().window;

    window.draw(mBackground);
    window.draw(mButtons);
    window.draw(mLines);

    return false;
}

bool LevelState::update(sf::Time)
{
    return false;
}

bool LevelState::handleEvent(const sf::Event& event)
{
    mButtons.handleEvent(event);
    return false;
}

void LevelState::createLevelButton(int index)
{
    // WIP
}

void LevelState::createConnectionLines()
{
    for(auto& levelButton : mLevelButtons)
    {
        // WIP
    }
}

void LevelState::createLine(sf::FloatRect ObjBox1, sf::FloatRect ObjBox2, int width)
{
    // WIP
}

LevelState::LevelButton::LevelButton(State::Context context, ButtonID id, const std::string& text, int levelID, bool locked)
    : GUIButton(context, id, text, locked)
{
}

int LevelState::LevelButton::getLevelID() const
{
    return mLevelID;
}
