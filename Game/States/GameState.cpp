#include "GameState.hpp"
#include "../MusicPlayer.hpp"

GameState::GameState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWorld(context),
      mPlayer(context.keys1, 0)
{
    context.music.playNow(Music::BattleTheme, true);
}

bool GameState::draw()
{
    mWorld.draw();
    return false;
}

bool GameState::update(sf::Time dt)
{
    mPlayer.handleRealTimeInput(mWorld.getCommandQueue());
    mWorld.update(dt);
    return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::PauseState);
    else
        mPlayer.handleEvent(event, mWorld.getCommandQueue());

    return false;
}
