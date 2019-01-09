#include "LevelState.hpp"
#include "../DataTable.hpp"
#include "../Profile.hpp"
#include "../GUI/GUIButton.hpp"
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
      mProfile(context.profile)
{
    for(int i=0; i < levelInfo.size(); ++i)
        createLevelButton(i);

    mLevelButtons[0]->setLocked(false); // First level should always be unlocked
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
    std::unique_ptr<LevelButton> levelButton(new LevelButton(getContext(), GUIButton::LevelButton, levelInfo[i].name, i));
    levelButton->setPosition(levelInfo[i].x, levelInfo[i].y);
    levelButton->centerButtonOrigin();
    levelButton->setCallback([this, i]()
    {
        if(mProfile.getCurrentLevel() != i) // Avoid deactivating itself
        {
            mLevelButtons[mProfile.getCurrentLevel()]->setFreezeFlag(false);
            mLevelButtons[mProfile.getCurrentLevel()]->deactivate();
        }
        mLevelButtons[i]->setFreezeFlag(true);
        mProfile.setCurrentLevel(i);
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

void LevelState::createLine(float point1, float point2, int width)
{
    // WIP
}
