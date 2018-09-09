#include "Player.hpp"

Player::Player(KeyBinding* keys, int id)
    : mKeyBinding(keys),
      mIdentifier(id)
{
}

void Player::handleRealTimeInput(CommandQueue& commands)
{

}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        Action action;
        if(mKeyBinding && mKeyBinding->findAction(event.key.code, action) && !isRealtimeAction(action))
        {
            // push command to queue
        }
    }
}
