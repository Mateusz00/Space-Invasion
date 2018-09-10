#include "Player.hpp"

struct AircraftMover // Command
{
    AircraftMover(float x, float y, int id)
        : velocity(x, y), identifier(id)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time dt)
    {
        if(aircraft.getIdentifier() == identifier)
            aircraft.accelerate(velocity * aircraft.getMaxSpeed());
    }

    sf::Vector2f velocity;
    int identifier;
};
Player::Player(KeyBinding* keys, int id)
    : mKeyBinding(keys),
      mIdentifier(id)
{
}

void Player::handleRealTimeInput(CommandQueue& commands)
{
    auto actions = mKeyBinding->getRealtimeActions();
    for(const auto& action : actions)
        commands.push(mActionBinding[action]);
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        Action action;
        if(mKeyBinding && mKeyBinding->findAction(event.key.code, action) && !isRealtimeAction(action))
        {
            commands.push(mActionBinding[action]);
        }
    }
}

void initializeActions()
{
	mActionBinding[Player::Action::MoveUp].action        = castFunctor<Aircraft>(AircraftMover(-1,  0, mIdentifier));
	mActionBinding[Player::Action::MoveDown].action      = castFunctor<Aircraft>(AircraftMover(+1,  0, mIdentifier));
	mActionBinding[Player::Action::MoveLeft].action      = castFunctor<Aircraft>(AircraftMover( 0, -1, mIdentifier));
	mActionBinding[Player::Action::MoveRight].action     = castFunctor<Aircraft>(AircraftMover( 0, +1, mIdentifier));
	mActionBinding[Player::Action::Fire].action          = castFunctor<Aircraft>(AircraftFireTrigger(mIdentifier));
	mActionBinding[Player::Action::LaunchMissile].action = castFunctor<Aircraft>(AircraftMissileTrigger(mIdentifier));
}
