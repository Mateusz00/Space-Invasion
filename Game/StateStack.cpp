#include "StateStack.hpp"
#include <exception>

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
            {
                auto foundFactory = mFactory.find(change.id);
                if(foundFactory == mFactory.end())
                    throw std::logic_error("There is no state with id: " + toString(static_cast<int>(change.id)));
                mStack.push_back((foundFactory->second)());
                break;
            }
            case Action::Pop:
                mStack.pop_back();
                break;

            case Action::Clear:
                mStack.clear();
                break;
        }
    }
}
