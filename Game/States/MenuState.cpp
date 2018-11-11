#include "MenuState.hpp"
#include "../ResourcesID.hpp"
#include "../MusicPlayer.hpp"
#include "../GUI/GUIButton.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

MenuState::MenuState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mSpeaker(context),
      mWindow(context.window),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen))
{
    context.music.playNow(Music::MenuTheme, true);

    sf::Vector2f windowSize(static_cast<sf::Vector2f>(mWindow.getSize()));
    mGUIContainer.setPosition(windowSize * 0.5f);

    std::unique_ptr<GUIButton> play(new GUIButton(context, GUIButton::Text, "Play"));
    play->setPosition(0.f, windowSize.y * -0.15f);
    play->setCallback([this]()
    {
        requestStackPop();
		requestStackPush(States::PlayerInfoState);
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
    exit->setPosition(0.f, windowSize.y * 0.15f);
    exit->setCallback([this]()
    {
        requestStackClear();
    });
    mGUIContainer.push(std::move(exit));

    sf::Vector2f mSpeakerPosition(static_cast<float>(mWindow.getSize().x) - mSpeaker.getSize().x, 0.f); // Left-top corner of window
    mSpeaker.setPosition(mSpeakerPosition - sf::Vector2f(8.f, -8.f));
}

bool MenuState::draw()
{
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mBackgroundSprite);
    mWindow.draw(mGUIContainer);
    mWindow.draw(mSpeaker);
    return false;
}

bool MenuState::update(sf::Time dt)
{
    return false;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.update(mWindow);
    mGUIContainer.handleEvent(event);
    mSpeaker.handleEvent(event);
    return false;
}
