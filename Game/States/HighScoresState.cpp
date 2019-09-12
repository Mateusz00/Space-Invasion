#include "HighScoresState.hpp"
#include "../ResourcesID.hpp"
#include "../Utility.hpp"
#include "../GUI/GUIButton.hpp"
#include "../GUI/GUI_InputBox.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdlib>

HighScoresState::HighScoresState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mHighScores(context.window, context.fonts),
      mBackground(context.textures.get(Textures::TitleScreen)),
      mHeader("High Scores", context.fonts.get(Fonts::Sansation), 48u)
{
    sf::Vector2f windowSize(static_cast<sf::Vector2f>(context.window.getSize()));

    mHighScores.setPosition(windowSize * 0.25f);
    centerOrigin(mHeader);
    mHeader.setPosition(windowSize.x * 0.5f, windowSize.y * 0.07f);
    mButtons.setPosition(windowSize.x * 0.5f, windowSize.y * 0.25f);

    auto returnButton = std::make_unique<GUIButton>(context, GUIButton::TextButton, "Return");
    returnButton->setPosition(0.f, mHighScores.getSize().y + windowSize.y * 0.05f);
    returnButton->setCallback([this]()
    {
        requestStackPop();
    });
    mButtons.push(std::move(returnButton));

    sf::Vector2f bSize(190.f, 30.f);
    auto inputBox = std::make_unique<GUI_InputBox>(mRequestedLevel, bSize, 3, false, context.fonts);

    sf::Vector2f bSize2(70.f, inputBox->getBoundingRect().height);
    sf::Vector2f bSize3(120.f, inputBox->getBoundingRect().height);
    float widthSum = bSize.x + bSize2.x + bSize3.x;

    inputBox->setPosition(-widthSum / 2.f, windowSize.y * -0.085f);
    inputBox->setHint("Enter level number..");

    auto updateButton = std::make_unique<GUIButton>(context, GUIButton::ScoresButton, "Update");
    updateButton->setRectSize(bSize2);
    updateButton->setPosition(inputBox->getPosition().x + bSize.x, windowSize.y * -0.085f - 1.f);
    updateButton->setCallback([this]()
    {
        int level = std::atoi(mRequestedLevel.c_str());
        if(level > 0)
            mHighScores.loadScores(level-1); // level 1 has id 0 etc.
    });

    auto overallScoreButton = std::make_unique<GUIButton>(context, GUIButton::ScoresButton, "Overall Score");
    overallScoreButton->setRectSize(bSize3);
    overallScoreButton->setPosition(updateButton->getPosition().x + bSize2.x, windowSize.y * -0.085f - 1.f);
    overallScoreButton->setCallback([this]()
    {
            mHighScores.loadScores(-1);
    });
    mButtons.push(std::move(updateButton));
    mButtons.push(std::move(overallScoreButton));
    mButtons.push(std::move(inputBox));
}

bool HighScoresState::draw()
{
    auto& window = getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackground);
    window.draw(mHighScores);
    window.draw(mHeader);
    window.draw(mButtons);

    return false;
}

bool HighScoresState::update(sf::Time dt)
{
    mButtons.update(dt);
    return false;
}

bool HighScoresState::handleEvent(const sf::Event& event)
{
    mButtons.handleEvent(event);
    return false;
}
