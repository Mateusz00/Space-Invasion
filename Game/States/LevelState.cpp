#include "LevelState.hpp"
#include "../DataTable.hpp"
#include <memory>

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
    for(int i=0; i < levelInfo.size(); ++i)
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

void LevelState::createLevelButton(int i)
{
    std::unique_ptr<LevelButton> levelButton(new LevelButton(getContext(), GUIButton::LevelButton, levelInfo[i].name, i, (i > 0) ? true : false));
    levelButton->setPosition(levelInfo[i].x, levelInfo[i].y);
    levelButton->setCallback([this]()
    {
        // TODO: Add Profile class and edit mCurrentLevel variable here
    });

    mLevelButtons.push_back(levelButton.get());
    mButtons.push(std::move(levelButton));
}

void LevelState::createConnectionLines()
{
    for(int i=0; i < mLevelButtons.size(); ++i)
    {
        // WIP
    }
}

void LevelState::loadSave()
{

}

void LevelState::createLine(float point1, float point2, int width)
{
    // WIP
}
