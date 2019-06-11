#include "Player.hpp"
#include "Entities/Spaceship.hpp"

struct SpaceshipSpeedBoost // Command
{
    SpaceshipSpeedBoost(int id) : identifier(id)
    {
    }

    void operator() (Spaceship& spaceship, sf::Time dt) const
    {
        if(spaceship.getIdentifier() == identifier)
            spaceship.boostSpeed();
    }
    int identifier;
};

struct SpaceshipMover // Command
{
    SpaceshipMover(float x, float y, int id)
        : velocity(x, y), identifier(id)
    {
    }

    void operator() (Spaceship& spaceship, sf::Time dt) const
    {
        if(spaceship.getIdentifier() == identifier)
            spaceship.accelerate(velocity * spaceship.getMaxSpeed());
    }
    sf::Vector2f velocity;
    int identifier;
};

struct SpaceshipFireTrigger // Command
{
    SpaceshipFireTrigger(int id) : identifier(id)
    {
    }

    void operator() (Spaceship& spaceship, sf::Time dt) const
    {
        if(spaceship.getIdentifier() == identifier)
            spaceship.fire();
    }
    int identifier;
};

struct SpaceshipMissileTrigger // Command
{
    SpaceshipMissileTrigger(int id) : identifier(id)
    {
    }

    void operator() (Spaceship& spaceship, sf::Time dt) const
    {
        if(spaceship.getIdentifier() == identifier)
             spaceship.launchMissile();
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
        actionBind.mCategories = Category::PlayerSpaceship;
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
    mActionBinding[Action::MoveUp].mAction        = castFunctor<Spaceship>(SpaceshipMover(0, -1, mIdentifier));
    mActionBinding[Action::MoveDown].mAction      = castFunctor<Spaceship>(SpaceshipMover(0,  1, mIdentifier));
    mActionBinding[Action::MoveLeft].mAction      = castFunctor<Spaceship>(SpaceshipMover(-1, 0, mIdentifier));
    mActionBinding[Action::MoveRight].mAction     = castFunctor<Spaceship>(SpaceshipMover(1,  0, mIdentifier));
    mActionBinding[Action::Fire].mAction          = castFunctor<Spaceship>(SpaceshipFireTrigger(mIdentifier));
    mActionBinding[Action::LaunchMissile].mAction = castFunctor<Spaceship>(SpaceshipMissileTrigger(mIdentifier));
    mActionBinding[Action::SpeedBoost].mAction    = castFunctor<Spaceship>(SpaceshipSpeedBoost(mIdentifier));
}
