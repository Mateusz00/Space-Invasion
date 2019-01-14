#include "LevelState.hpp"
#include "../DataTable.hpp"
#include "../Profile.hpp"
#include "../Utility.hpp"
#include "../GUI/GUIButton.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

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
    window.draw(mLines);
    window.draw(mButtons);

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
        const std::vector<int>& levelDependencies = levelInfo[i].levelDependencies;

        // Get first point of every line segment(center of the button)
        sf::FloatRect box = mLevelButtons[i]->getBoundingRect();
        sf::Vector2f center = sf::Vector2f(box.left + box.width*0.5f, box.top + box.height*0.5f);

        // Get second point and create line segment for all dependencies
        for(const auto& levelID : levelDependencies)
        {
            sf::FloatRect box = mLevelButtons[levelID]->getBoundingRect();
            sf::Vector2f center2(box.left + box.width*0.5f, box.top + box.height*0.5f);
            createLine(center, center2, 6.f);
        }
    }
}

void LevelState::createLine(sf::Vector2f point1, sf::Vector2f point2, float width)
{
    sf::Vector2f unitDirection = unitVector(point1 - point2);
    sf::Vector2f perpendicularVector(-unitDirection.y, unitDirection.x);
    sf::Vector2f offset = (width * 0.5f) * perpendicularVector;

    // 1st triangle
    mLines.append(sf::Vertex(point1 - offset, sf::Color(203, 198, 206)));
    mLines.append(sf::Vertex(point1 + offset, sf::Color(203, 198, 206)));
    mLines.append(sf::Vertex(point2 - offset, sf::Color(203, 198, 206)));

    // 2nd triangle
    mLines.append(sf::Vertex(point2 - offset, sf::Color(203, 198, 206)));
    mLines.append(sf::Vertex(point2 + offset, sf::Color(203, 198, 206)));
    mLines.append(sf::Vertex(point1 + offset, sf::Color(203, 198, 206)));
}
