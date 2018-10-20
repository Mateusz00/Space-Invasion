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
    // Add commands system
    mPlayer.handleEvent(event, mWorld.getCommandQueue());
    // Add PauseState push event
    return false;
}
