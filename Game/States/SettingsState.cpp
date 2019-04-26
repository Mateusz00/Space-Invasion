#include "SettingsState.hpp"
#include "../GUI/GUISlider.hpp"
#include "../GUI/GUISwitch.hpp"
#include "../SoundPlayer.hpp"
#include "../MusicPlayer.hpp"

SettingsState::SettingsState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen)),
      mWindow(context.window),
      mOptionButtons(true, false),
      mOtherOptions(false),
      mCurrentOption(Controls),
      mSettings(context.settings)
{
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(mWindow.getSize());

    mContainer.setSize(sf::Vector2f(windowSize.x * 0.8f, windowSize.y * 0.9f));
    mContainer.setPosition(sf::Vector2f(windowSize.x * 0.1f, windowSize.y * 0.05f));
    mContainer.setFillColor(sf::Color::Black);
    mContainer.setOutlineColor(sf::Color(183, 124, 7));
    mContainer.setOutlineThickness(1.f);

    mBottomBar.setSize(sf::Vector2f(mContainer.getSize().x, mContainer.getSize().y * 0.08f));
    mBottomBar.setPosition(sf::Vector2f(mContainer.getPosition().x, mContainer.getPosition().y  + mContainer.getSize().y - mBottomBar.getSize().y));
    mBottomBar.setFillColor(sf::Color(47, 47, 48));

    mOptionButtons.setPosition(mContainer.getPosition());
    mControls.setPosition(sf::Vector2f(windowSize.x * 0.15f, windowSize.y * 0.175f));
    mOtherOptions.setPosition(sf::Vector2f(windowSize.x * 0.15f, windowSize.y * 0.2f));

    addGUIElements(context);
}

bool SettingsState::draw()
{
    mWindow.draw(mBackgroundSprite);
    mWindow.draw(mContainer);
    mWindow.draw(mBottomBar);
    mWindow.draw(mOptionButtons);

    switch(mCurrentOption)
    {
        case Controls:
            mWindow.draw(mControls);
            break;

        case Others:
            mWindow.draw(mOtherOptions);
            break;
    }

    return false;
}

bool SettingsState::update(sf::Time dt)
{
    return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
    if(mToggledButton.first == true)
    {
        int index = mToggledButton.second;
        if(event.type == sf::Event::KeyReleased)
        {
            int i = static_cast<int>(index / KeyBinding::Count); // Deduce which KeyBinding should be modified
            if(i < getContext().keys.size())
                getContext().keys[i]->assignKey(static_cast<KeyBinding::Action>(index % KeyBinding::Count), event.key.code);

            mBindingButtons[index]->deactivate();
            updateLabels();
            mToggledButton.first = false;
        }
    }
    else
    {
        mOptionButtons.handleEvent(event);
        switch(mCurrentOption)
        {
            case Controls:
                mControls.handleEvent(event);
                break;

            case Others:
                mOtherOptions.handleEvent(event);
                break;
        }
    }

    return false;
}

void SettingsState::updateLabels()
{
    for(int i=0; i < KeyBinding::Count; ++i)
    {
        auto action = static_cast<Player::Action>(i);

        for(int j=0; j < getContext().keys.size(); ++j) // For each player
        {
            sf::Keyboard::Key key = getContext().keys[j]->getAssignedKey(action);
            mBindingLabels[i + KeyBinding::Count*j]->setText(toString(key));
        }
    }
}

void SettingsState::addButton(int index, int player, int y, const std::string& text, Context context)
{
    index += KeyBinding::Count * player;

    std::unique_ptr<GUIButton> newButton(new GUIButton(context, GUIButton::ControlsButton, text));
    mBindingButtons[index] = newButton.get();
    newButton->setPosition(360.f * player, 70.f * y);
    newButton->setCallback([index, this]()
    {
        mBindingButtons[index]->toggle(true);
        mToggledButton = std::make_pair(true, static_cast<Player::Action>(index));
    });
    mControls.push(std::move(newButton));

    std::unique_ptr<GUILabel> newLabel(new GUILabel(text, context.fonts));
    mBindingLabels[index] = newLabel.get();
    newLabel->setPosition(230.f + 360.f * player, 70.f * y + 10.f);
    mControls.push(std::move(newLabel));
}

void SettingsState::addGUIElements(Context context)
{
    for(int player=0; player < 2; ++player)
    {
        addButton(KeyBinding::MoveLeft,         player, 0, "Move Left", context);
        addButton(KeyBinding::MoveRight,        player, 1, "Move Right", context);
        addButton(KeyBinding::MoveUp,           player, 2, "Move Up", context);
        addButton(KeyBinding::MoveDown,         player, 3, "Move Down", context);
        addButton(KeyBinding::Fire,             player, 4, "Fire", context);
        addButton(KeyBinding::LaunchMissile,    player, 5, "Missile", context);
        addButton(KeyBinding::SpeedBoost,       player, 6, "Speed Boost", context);
    }
    updateLabels();

    std::unique_ptr<GUIButton> returnButton(new GUIButton(context, GUIButton::TextButton, "Return"));
    returnButton->setAbsolutePosition();
    returnButton->setPosition(context.window.getSize().x * 0.35f, context.window.getSize().y * 0.9f);
    returnButton->setCallback([this]()
    {
        requestStackPop();
    });

    std::unique_ptr<GUIButton> saveButton(new GUIButton(context, GUIButton::TextButton, "Save"));
    saveButton->setAbsolutePosition();
    saveButton->setPosition(context.window.getSize().x * 0.65f, context.window.getSize().y * 0.9f);
    saveButton->setCallback([this, context]()
    {
        context.settings.saveToFile();
    });

    // Add and position buttons
    std::unique_ptr<GUIButton> controls(new GUIButton(context, GUIButton::OptionsButton, "Controls"));
    GUIButton* controlsPtr = controls.get();
    controls->setPosition(0.f, 0.f);
    controls->changeAppearance(GUIButton::Pressed);
    controls->setFreezeFlag(true);
    controls->setRectSize(sf::Vector2f(mContainer.getSize().x * 0.5f - 2.f, mBottomBar.getSize().y));

    std::unique_ptr<GUIButton> others(new GUIButton(context, GUIButton::OptionsButton, "Others"));
    GUIButton* othersPtr = others.get();
    others->setPosition(sf::Vector2f(mContainer.getSize().x * 0.5f, 0.f));
    others->setRectSize(sf::Vector2f(mContainer.getSize().x * 0.5f, mBottomBar.getSize().y));

    // set callbacks
    controls->setCallback([this, controlsPtr, othersPtr]()
    {
        mCurrentOption = Controls;
        othersPtr->setFreezeFlag(false);
        othersPtr->deactivate();
        othersPtr->deselect();
        controlsPtr->setFreezeFlag(true);
    });
    others->setCallback([this, controlsPtr, othersPtr]()
    {
        mCurrentOption = Others;
        controlsPtr->setFreezeFlag(false);
        controlsPtr->deactivate();
        controlsPtr->deselect();
        othersPtr->setFreezeFlag(true);
    });

    mOptionButtons.push(std::move(controls));
    mOptionButtons.push(std::move(others));
    mOptionButtons.push(std::move(returnButton));
    mOptionButtons.push(std::move(saveButton));

    std::unique_ptr<GUILabel> soundText(new GUILabel("Sound", context.fonts));
    soundText->setPosition(0.f, 0.f);
    mOtherOptions.push(std::move(soundText));

    std::unique_ptr<GUISlider> soundEffects(new GUISlider(250.f, 30.f, context.sounds.getVolume(), context.fonts, &mOtherOptions));
    soundEffects->setPosition(100.f, 0.f);
    soundEffects->setValueRange(0.f, 100.f);
    soundEffects->setUpdatingFunction([this, context](float value)
    {
        context.sounds.setVolume(value);
    });
    mOtherOptions.push(std::move(soundEffects));

    std::unique_ptr<GUILabel> musicText(new GUILabel("Music", context.fonts));
    musicText->setPosition(0.f, 150.f);
    mOtherOptions.push(std::move(musicText));

    std::unique_ptr<GUISlider> musicVolume(new GUISlider(250.f, 30.f, context.music.getVolume(), context.fonts, &mOtherOptions));
    musicVolume->setPosition(100.f, 150.f);
    musicVolume->setValueRange(0.f, 100.f);
    musicVolume->setUpdatingFunction([this, context](float value)
    {
        context.music.setVolume(value);
    });
    mOtherOptions.push(std::move(musicVolume));

    std::unique_ptr<GUILabel> vsyncText(new GUILabel("V-sync", context.fonts));
    vsyncText->setPosition(0.f, 250.f);
    mOtherOptions.push(std::move(vsyncText));

    std::unique_ptr<GUISwitch> vsync(new GUISwitch(context, Textures::Checkbox, mSettings.getVsync()));
    vsync->setPosition(100.f, 250.f);
    vsync->setOnCallback([this, context]()
    {
        context.window.setVerticalSyncEnabled(true);
        mSettings.setVsync(true);
    });
    vsync->setOffCallback([this, context]()
    {
        context.window.setVerticalSyncEnabled(false);
        mSettings.setVsync(false);
    });
    mOtherOptions.push(std::move(vsync));

    std::unique_ptr<GUILabel> muteText(new GUILabel("Mute", context.fonts));
    muteText->setPosition(0.f, 350.f);
    mOtherOptions.push(std::move(muteText));

    std::unique_ptr<GUISwitch> mute(new GUISwitch(context, Textures::Checkbox, mSettings.getMuted()));
    mute->setPosition(100.f, 350.f);
    mute->setOnCallback([this, context]()
    {
        context.music.mute();
        context.sounds.mute();
        mSettings.setMuted(true);
    });
    mute->setOffCallback([this, context]()
    {
        context.music.unmute();
        context.sounds.unmute();
        mSettings.setMuted(false);
    });
    mOtherOptions.push(std::move(mute));
}
