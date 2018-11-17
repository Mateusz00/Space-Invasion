#include "HighScoresState.hpp"
#include "../ResourcesID.hpp"
#include "../Utility.hpp"
#include "../GUI/GUIButton.hpp"
#include <SFML/System/Vector2.hpp>

HighScoresState::HighScoresState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mHighScores(context.window, context.fonts),
      mBackground(context.textures.get(Textures::TitleScreen)),
      mHeader("High Scores", context.fonts.get(Fonts::Sansation), 48u)
{
    sf::Vector2f windowSize(static_cast<sf::Vector2f>(context.window.getSize()));

    mHighScores.setPosition(windowSize * 0.25f);
    centerOrigin(mHeader);
	mHeader.setPosition(windowSize.x * 0.5f, windowSize.y * 0.15f);
	mButtons.setPosition(windowSize.x * 0.5f, windowSize.y * 0.9f);

    std::unique_ptr<GUIButton> returnButton(new GUIButton(context, GUIButton::Text, "Return"));
    returnButton->setPosition(0.f, 0.f);
    returnButton->setCallback([this]()
    {
        requestStackPop();
    });
    mButtons.push(std::move(returnButton));
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
    mButtons.update(getContext().window);
    return false;
}

bool HighScoresState::handleEvent(const sf::Event& event)
{
    mButtons.handleEvent(event);
    return false;
}
