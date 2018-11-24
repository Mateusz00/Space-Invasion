#include "MenuState.hpp"
#include "../ResourcesID.hpp"
#include "../MusicPlayer.hpp"
#include "../GUI/GUIButton.hpp"
#include "../GUI/GUISwitch.hpp"
#include "../SoundPlayer.hpp"
#include "../MusicPlayer.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

MenuState::MenuState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWindow(context.window),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen))
{
    context.music.playNow(Music::MenuTheme, true);

    sf::Vector2f windowSize(static_cast<sf::Vector2f>(mWindow.getSize()));
    mGUIContainer.setPosition(windowSize.x * 0.5f, windowSize.y * 0.45f);

    std::unique_ptr<GUIButton> play(new GUIButton(context, GUIButton::TextButton, "Play"));
    play->setPosition(0.f, windowSize.y * -0.12f);
    play->setCallback([this]()
    {
        requestStackPop();
		requestStackPush(States::PlayerInfoState);
    });
    mGUIContainer.push(std::move(play));

    std::unique_ptr<GUIButton> settings(new GUIButton(context, GUIButton::TextButton, "Settings"));
    settings->setPosition(0.f, 0.f);
    settings->setCallback([this]()
    {
        requestStackPush(States::SettingsState);
    });
    mGUIContainer.push(std::move(settings));

    std::unique_ptr<GUIButton> HighScores(new GUIButton(context, GUIButton::TextButton, "High Scores"));
    HighScores->setPosition(0.f, windowSize.y * 0.12f);
    HighScores->setCallback([this]()
    {
        requestStackPush(States::HighScoresState);
    });
    mGUIContainer.push(std::move(HighScores));

    std::unique_ptr<GUIButton> exit(new GUIButton(context, GUIButton::TextButton, "Exit"));
    exit->setPosition(0.f, windowSize.y * 0.24f);
    exit->setCallback([this]()
    {
        requestStackClear();
    });
    mGUIContainer.push(std::move(exit));

    std::unique_ptr<GUISwitch> speaker(new GUISwitch(context, Textures::Speaker));
    sf::Vector2f mSpeakerPosition(static_cast<float>(mWindow.getSize().x) - speaker->getBoundingRect().width, 0.f); // top-right
    speaker->setPosition(mSpeakerPosition - sf::Vector2f(8.f, -8.f)); // Margin
    speaker->setAbsolutePosition();
    speaker->setOnCallback([this, context]()
    {
        context.sounds.unmute();
        context.music.unmute();
    });
    speaker->setOffCallback([this, context]()
    {
        context.sounds.mute();
        context.music.mute();
    });
    mGUIContainer.push(std::move(speaker));
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
    return false;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}
