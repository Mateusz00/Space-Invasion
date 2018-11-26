#include "Player.hpp"
#include "Entities/Aircraft.hpp"

struct AircraftMover // Command
{
    AircraftMover(float x, float y, int id)
        : velocity(x, y), identifier(id)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time dt) const
    {
        if(aircraft.getIdentifier() == identifier)
            aircraft.accelerate(velocity * aircraft.getMaxSpeed());
    }
    sf::Vector2f velocity;
    int identifier;
};

struct AircraftFireTrigger // Command
{
    AircraftFireTrigger(int id) : identifier(id)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time dt) const
    {
        if(aircraft.getIdentifier() == identifier)
            aircraft.fire();
    }
    int identifier;
};

struct AircraftMissileTrigger // Command
{
    AircraftMissileTrigger(int id) : identifier(id)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time dt) const
    {
        if(aircraft.getIdentifier() == identifier)
             aircraft.launchMissile();
    }
    int identifier;
};


Player::Player(KeyBinding* keys, int id, std::string& name)
    : mIdentifier(id),
      mScore(0),
      mKeyBinding(keys),
      mName(name)
{
    initializeActions();

    for(auto& actionBind : mActionBinding)
        actionBind.mCategories.push_back(Category::PlayerAircraft);
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

void Player::setScore(int value)
{
    mScore = value;
}

int Player::getScore() const
{
    return mScore;
}

int Player::getID() const
{
    return mIdentifier;
}
void Player::setName(const std::string& name)
{
    mName = name;
}

const std::string& Player::getName() const
{
    return mName;
}

void Player::initializeActions()
{
    mActionBinding[Action::MoveUp].mAction        = castFunctor<Aircraft>(AircraftMover(0, -1, mIdentifier));
    mActionBinding[Action::MoveDown].mAction      = castFunctor<Aircraft>(AircraftMover(0,  1, mIdentifier));
    mActionBinding[Action::MoveLeft].mAction      = castFunctor<Aircraft>(AircraftMover(-1, 0, mIdentifier));
    mActionBinding[Action::MoveRight].mAction     = castFunctor<Aircraft>(AircraftMover(1,  0, mIdentifier));
    mActionBinding[Action::Fire].mAction          = castFunctor<Aircraft>(AircraftFireTrigger(mIdentifier));
    mActionBinding[Action::LaunchMissile].mAction = castFunctor<Aircraft>(AircraftMissileTrigger(mIdentifier));
}
