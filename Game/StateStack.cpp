#include "StateStack.hpp"
StateStack::PendingChange::PendingChange(Action action, States::ID id)
    : action(action),
      id(id)
{
}

StateStack::StateStack(State::Context context)
    : mContext(context)
{
}

void StateStack::update(sf::Time dt)
{
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) // Iterate from last to first element (FIFO)
    {
        if(!(*itr)->update(dt)) // if function returns false then it means that this state blocks other states
            break;
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if(!(*itr)->draw())
            break;
    }
}

void StateStack::handleEvent(const sf::Event& event)
{
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if(!(*itr)->handleEvent(event))
            break;
    }
    applyPendingChanges();
}

void StateStack::pushState(States::ID id)
{
    mPendingChanges.push_back(PendingChange(Action::Push, id));
}

void StateStack::popState()
{
    mPendingChanges.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
    mPendingChanges.push_back(PendingChange(Action::Clear));
}

void StateStack::applyPendingChanges()
{
    for(auto& change : mPendingChanges)
    {
        switch(change.action) // TODO: Add template function that creates state corresponding to stateID and finish this function
        {
            case Action::Push:
                break;
            case Action::Pop:
                break;
            case Action::Clear:
                break;
        }
    }
}
