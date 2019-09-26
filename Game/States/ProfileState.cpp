#include "ProfileState.hpp"
#include "../GUI/GUIButton.hpp"
#include "../Profile.hpp"
#include "../ResourcesID.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>

ProfileState::ProfileState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mProfile(context.profile),
      mBackground(context.textures.get(Textures::TitleScreen))
{
    sf::Vector2f windowSize(static_cast<sf::Vector2f>(context.window.getSize()));

    std::unique_ptr<GUIButton> newGame(new GUIButton(context, GUIButton::TextButton, "New Game"));
    newGame->setPosition(0.f, windowSize.y * -0.06f);
    newGame->setCallback([this]()
    {
        mProfile.clearAllData();
        requestStackPop();
        requestStackPush(States::PlayerInfoState);
    });

    std::unique_ptr<GUIButton> loadGame(new GUIButton(context, GUIButton::TextButton, "Load Game"));
    loadGame->setPosition(0.f, windowSize.y * 0.06f);
    loadGame->setCallback([this]()
    {
        if(mProfile.loadProfile())
        {
            requestStackPop();
            requestStackPush(States::LevelState);
        }
        else
        {
            requestStackPop();
            requestStackPush(States::PlayerInfoState);
        }
    });

    mOptions.push(std::move(newGame));
    mOptions.push(std::move(loadGame));
    mOptions.setPosition(static_cast<sf::Vector2f>(context.window.getSize()) * 0.5f);
}

bool ProfileState::draw()
{
    getContext().window.draw(mBackground);
    getContext().window.draw(mOptions);
}

bool ProfileState::update(sf::Time)
{
}

bool ProfileState::handleEvent(const sf::Event& event)
{
    mOptions.handleEvent(event);
}
