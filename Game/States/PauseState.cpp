#include "PauseState.hpp"
#include "../ResourcesID.hpp"
#include "../MusicPlayer.hpp"
#include "../GUI/GUIButton.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

PauseState::PauseState(Context context, StateStack& stateStack, bool isNetworked)
    : State(context, stateStack),
      mWindow(context.window),
      mIsNetworked(isNetworked)
{
    sf::Vector2f windowSize(static_cast<sf::Vector2f>(mWindow.getSize()));

    mText.setFont(context.fonts.get(Fonts::Sansation));
    mText.setString("Game Paused");
    mText.setCharacterSize(60u);
    centerOrigin(mText);
    mText.setPosition(windowSize.x * 0.5f, windowSize.y * 0.3f);

    mGUIContainer.setPosition(windowSize * 0.5f);

    std::unique_ptr<GUIButton> returnButton(new GUIButton(context, GUIButton::TextButton, "Return"));
    returnButton->setPosition(0.f, -windowSize.y * 0.05f);
    returnButton->setCallback([this]()
    {
        getContext().music.resume();
        requestStackPop();
    });
    mGUIContainer.push(std::move(returnButton));

    std::unique_ptr<GUIButton> levelButton(new GUIButton(context, GUIButton::TextButton, "Choose level"));
    levelButton->setPosition(0.f, windowSize.y * 0.07f);
    levelButton->setCallback([this, context]()
    {
        requestStackClear();
        requestStackPush(States::LevelState);
        context.music.playNow(Music::MenuTheme, true);
    });
    mGUIContainer.push(std::move(levelButton));

    std::unique_ptr<GUIButton> backToMenu(new GUIButton(context, GUIButton::TextButton, "Menu"));
    backToMenu->setPosition(0.f, windowSize.y * 0.19f);
    backToMenu->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::MenuState);
    });
    mGUIContainer.push(std::move(backToMenu));

    context.music.pause();
}

bool PauseState::draw()
{
    sf::RectangleShape background; // Optimize
    background.setFillColor(sf::Color(0, 0, 0, 150));
    background.setSize(mWindow.getView().getSize());

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(background);
    mWindow.draw(mText);
    mWindow.draw(mGUIContainer);

    return mIsNetworked;
}

bool PauseState::update(sf::Time dt)
{
    return mIsNetworked;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}
