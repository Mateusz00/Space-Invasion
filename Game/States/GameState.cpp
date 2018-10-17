#include "GameState.hpp"

GameState::GameState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWorld(context.window, context.textures, context.fonts, context.sounds),
      mPlayer(context.keys1, 0)
{
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
