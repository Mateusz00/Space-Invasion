#include "SettingsState.hpp"

SettingsState::SettingsState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen)),
      mWindow(context.window)
{
    for(int player=0; player < 2; ++player)
    {
        addButton(KeyBinding::MoveLeft,         player, 0, "Move Left", context);
        addButton(KeyBinding::MoveRight,        player,	1, "Move Right", context);
        addButton(KeyBinding::MoveUp,           player,	2, "Move Up", context);
        addButton(KeyBinding::MoveDown,         player, 3, "Move Down", context);
        addButton(KeyBinding::Fire,             player, 4, "Fire", context);
        addButton(KeyBinding::LaunchMissile,    player, 5, "Missile", context);
    }
    updateLabels();

    std::unique_ptr<GUIButton> returnButton(new GUIButton(context, GUIButton::Text, "Return"));
    returnButton->setPosition(500.f, 650.f);
    returnButton->setCallback([this]()
    {
        requestStackPop();
    });
    mGUIContainer.push(std::move(returnButton));
}

bool SettingsState::draw()
{
	mWindow.draw(mBackgroundSprite);
	mWindow.draw(mGUIContainer);
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
            for(int i=1; i <= getContext().keys.size(); ++i) // Deduce which KeyBinding should be modified
            {
                if(index < KeyBinding::Count * i)
                {
                    getContext().keys[i-1]->assignKey(static_cast<KeyBinding::Action>(index % KeyBinding::Count), event.key.code);
                    break;
                }
            }

            mBindingButtons[index]->deactivate();
            updateLabels();
            mToggledButton.first = false;
        }
    }
    else
    {
        mGUIContainer.update(mWindow);
        mGUIContainer.handleEvent(event);
    }

    return false;
}

void SettingsState::updateLabels()
{
    for(int i=0; i < KeyBinding::Count; ++i)
	{
		auto action = static_cast<Player::Action>(i);

		for(int j=0; j < getContext().keys.size(); ++j)
        {
            sf::Keyboard::Key key = getContext().keys[j]->getAssignedKey(action);
            mBindingLabels[i + KeyBinding::Count*j]->setText(toString(key));
        }
	}
}

void SettingsState::addButton(int index, int player, int y, const std::string& text, Context context)
{
    index += KeyBinding::Count * player;

    std::unique_ptr<GUIButton> newButton(new GUIButton(context, GUIButton::Settings, text));
    mBindingButtons[index] = newButton.get();
	newButton->setPosition(80.f + 450.f * player, 80.f * y + 120.f);
	newButton->setCallback([index, this]()
    {
        mBindingButtons[index]->toggle(true);
        mToggledButton = std::make_pair(true, static_cast<Player::Action>(index));
    });
    mGUIContainer.push(std::move(newButton));

	std::unique_ptr<GUILabel> newLabel(new GUILabel(text, context.fonts));
	mBindingLabels[index] = newLabel.get();
	newLabel->setPosition(350.f + 450.f * player, 80.f * y + 130.f);
	mGUIContainer.push(std::move(newLabel));
}
