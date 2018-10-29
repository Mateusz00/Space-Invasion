#include "SettingsState.hpp"

SettingsState::SettingsState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen)),
      mWindow(context.window)
{
    addButton(KeyBinding::MoveLeft,         0, "Move Left", context);
    addButton(KeyBinding::MoveRight,	    1, "Move Right", context);
    addButton(KeyBinding::MoveUp,		    2, "Move Up", context);
    addButton(KeyBinding::MoveDown,	        3, "Move Down", context);
    addButton(KeyBinding::Fire,		        4, "Fire", context);
    addButton(KeyBinding::LaunchMissile,    5, "Missile", context);
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
            getContext().keys1->assignKey(static_cast<KeyBinding::Action>(index), event.key.code);
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
    for(int i = 0; i < KeyBinding::Count; ++i)
	{
		auto action = static_cast<Player::Action>(i);
		sf::Keyboard::Key key1 = getContext().keys1->getAssignedKey(action);
		mBindingLabels[i]->setText(toString(key1));
	}
}

void SettingsState::addButton(int index, int y, const std::string& text, Context context)
{
    std::unique_ptr<GUIButton> newButton(new GUIButton(context, GUIButton::Settings, text));
    mBindingButtons[index] = newButton.get();
	newButton->setPosition(80.f, 80.f * y + 50.f);
	newButton->setCallback([index, this]()
    {
        mBindingButtons[index]->toggle(true);
        mToggledButton = std::make_pair(true, static_cast<Player::Action>(index));
    });
    mGUIContainer.push(std::move(newButton));

	std::unique_ptr<GUILabel> newLabel(new GUILabel(text, context.fonts));
	mBindingLabels[index] = newLabel.get();
	newLabel->setPosition(350.f, 80.f * y + 60.f);
	mGUIContainer.push(std::move(newLabel));
}
