#include "LevelState.hpp"
#include "../DataTable.hpp"

namespace
{
    const std::vector<LevelData> levelInfo = initializeLevelData();
}

LevelState::LevelState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mBackground(context.textures.get(Textures::TitleScreen)),
      mButtons(true, false),
      mLines(sf::Triangles),
      mLevel(0) //temp solution
{
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
