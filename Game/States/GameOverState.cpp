#include "GameOverState.hpp"
#include "../Utility.hpp"
#include "../GUI/GUIButton.hpp"
#include "../ResourceHolder.hpp"
#include "../MusicPlayer.hpp"
#include <SFML/Graphics/View.hpp>
#include <string>

GameOverState::GameOverState(Context context, StateStack& stateStack, Type type)
    : State(context, stateStack),
      mWindow(context.window)
{
    std::string text((type == Success) ? "Mission Success" : "Mission Failed");
    mText.setString(text);
    mText.setFont(context.fonts.get(Fonts::Sansation));
    mText.setCharacterSize(60u);
    centerOrigin(mText);

    sf::Vector2f windowSize(context.window.getSize());
    mText.setPosition(0.5f * windowSize.x, 0.35f * windowSize.y);
    mButtons.setPosition(0.5f * windowSize.x, 0.55f * windowSize.y);

    mBackground.setFillColor(sf::Color(0, 0, 0, 140));
    mBackground.setSize(mWindow.getView().getSize());

    std::unique_ptr<GUIButton> retryButton(new GUIButton(context, GUIButton::TextButton, "Retry"));
    retryButton->setPosition(0.f, -windowSize.y * 0.05f);
    retryButton->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::GameState);
    });
    mButtons.push(std::move(retryButton));

    std::unique_ptr<GUIButton> levelButton(new GUIButton(context, GUIButton::TextButton, "Choose level"));
    levelButton->setPosition(0.f, windowSize.y * 0.07f);
    levelButton->setCallback([this, context]()
    {
        requestStackClear();
        requestStackPush(States::LevelState);
        context.music.playNow(Music::MenuTheme, true);
    });
    mButtons.push(std::move(levelButton));

    std::unique_ptr<GUIButton> menuButton(new GUIButton(context, GUIButton::TextButton, "Menu"));
    menuButton->setPosition(0.f, windowSize.y * 0.19f);
    menuButton->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::MenuState);
    });
    mButtons.push(std::move(menuButton));
}

bool GameOverState::draw()
{
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mBackground);
    mWindow.draw(mText);
    mWindow.draw(mButtons);
    return false;
}

bool GameOverState::update(sf::Time dt)
{
    mElapsedTime += dt;
    if(mElapsedTime > sf::seconds(6))
    {
        requestStackClear();
        requestStackPush(States::LevelState);
    }

    return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
    mButtons.handleEvent(event);
    return false;
}
