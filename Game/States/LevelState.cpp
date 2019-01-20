#include "LevelState.hpp"
#include "../DataTable.hpp"
#include "../Profile.hpp"
#include "../Utility.hpp"
#include "../GUI/GUIButton.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

namespace level
{
    const std::vector<LevelData> levelInfo = initializeLevelData();
}

LevelState::LevelState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mBackground(context.textures.get(Textures::TitleScreen)),
      mButtons(true, false),
      mLines(sf::Triangles),
      mProfile(context.profile),
      mErrorMsg(context.fonts.get(Fonts::Goodfish), "Please choose level", 32u)
{
    for(int i=0; i < level::levelInfo.size(); ++i)
        createLevelButton(i);

    mLevelButtons[0]->setLocked(false); // First level should always be unlocked
    createConnectionLines();

    const sf::Vector2f windowSize = static_cast<sf::Vector2f>(context.window.getSize());
    mErrorMsg.setBarSize(sf::Vector2f(windowSize.x, windowSize.y * 0.2f));
    mErrorMsg.setPosition(windowSize * 0.5f);
    mErrorMsg.centerOrigin();
    mErrorMsg.setFadeCooldown(sf::seconds(1.5f));
    mErrorMsg.setFadeTime(sf::seconds(1.5f));
    mErrorMsg.setTextFillColor(sf::Color(239, 240, 242, 230));
    mErrorMsg.setBarFillColor(sf::Color(20, 20, 20, 220));

    std::unique_ptr<GUIButton> confirm(new GUIButton(context, GUIButton::ControlsButton, "Confirm"));
    confirm->setPosition(windowSize.x * 0.5f, windowSize.y * 0.92f);
    confirm->centerButtonOrigin();
    confirm->setCallback([this, context]()
    {
        if(context.profile.getCurrentLevel() >= 0)
        {
            requestStackClear();
            requestStackPush(States::GameState);
        }
        else
            mErrorMsg.setActive(true);
    });
    mButtons.push(std::move(confirm));

    std::unique_ptr<GUIButton> save(new GUIButton(context, GUIButton::ControlsButton, "Save"));
    save->setPosition(windowSize.x * 0.8f, windowSize.y * 0.92f);
    save->centerButtonOrigin();
    save->setCallback([this]()
    {
        mProfile.saveProfile();
    });
    mButtons.push(std::move(save));

    std::unique_ptr<GUIButton> menu(new GUIButton(context, GUIButton::ControlsButton, "Menu"));
    menu->setPosition(windowSize.x * 0.2f, windowSize.y * 0.92f);
    menu->centerButtonOrigin();
    menu->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::MenuState);
    });
    mButtons.push(std::move(menu));
}

bool LevelState::draw()
{
    auto& window = getContext().window;

    window.draw(mBackground);
    window.draw(mLines);
    window.draw(mButtons);
    window.draw(mErrorMsg);

    return false;
}

bool LevelState::update(sf::Time dt)
{
    mErrorMsg.update(dt);
    return false;
}

bool LevelState::handleEvent(const sf::Event& event)
{
    mButtons.handleEvent(event);
    return false;
}

void LevelState::createLevelButton(int i)
{
    std::unique_ptr<LevelButton> levelButton(new LevelButton(getContext(), GUIButton::LevelButton, level::levelInfo[i].name, i));
    levelButton->setPosition(level::levelInfo[i].x, level::levelInfo[i].y);
    levelButton->centerButtonOrigin();
    levelButton->setCallback([this, i]()
    {
        if(mProfile.getCurrentLevel() != i && mProfile.getCurrentLevel() >= 0) // Avoid deactivating itself
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
        const std::vector<int>& levelDependencies = level::levelInfo[i].levelDependencies;

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
