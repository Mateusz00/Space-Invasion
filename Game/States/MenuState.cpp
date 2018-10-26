#include "MenuState.hpp"
#include "../ResourcesID.hpp"
#include "../MusicPlayer.hpp"
#include "../GUI/GUIButton.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

MenuState::MenuState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWindow(context.window),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen))
{
    context.music.playNow(Music::MenuTheme, true);
    mGUIContainer.setPosition(static_cast<sf::Vector2f>((mWindow.getSize() / 2u)));

    std::unique_ptr<GUIButton> play(new GUIButton(context, GUIButton::Text, "Play"));
    play->setPosition(0.f, -100.f);
    play->setCallback([this]()
    {
        requestStackPop();
		requestStackPush(States::GameState);
    });
    mGUIContainer.push(std::move(play));

    std::unique_ptr<GUIButton> settings(new GUIButton(context, GUIButton::Text, "Settings"));
    settings->setPosition(0.f, 0.f);
    settings->setCallback([this]()
    {
        requestStackPush(States::SettingsState);
    });
    mGUIContainer.push(std::move(settings));

    std::unique_ptr<GUIButton> exit(new GUIButton(context, GUIButton::Text, "Exit"));
    exit->setPosition(0.f, 100.f);
    exit->setCallback([this]()
    {
        requestStateClear();
    });
    mGUIContainer.push(std::move(exit));
}

bool MenuState::draw()
{
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mBackgroundSprite);
    mWindow.draw(mGUIContainer);
    return false;
}

bool MenuState::update(sf::Time dt)
{
    mGUIContainer.update(mWindow);
    return false;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}
