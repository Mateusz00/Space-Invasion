#include "GameState.hpp"

GameState::GameState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWorld(context.window, context.textures, context.fonts)
{
}

bool GameState::draw()
{
    mWorld.draw();
    return false;
}

bool GameState::update(sf::Time dt)
{
    mPlayer.handleRealTimeInput();
    mWorld.update();
    return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
    // Add commands system
    mPlayer.handleEvent(event);
    // Add PauseState push event
    return false;
}
