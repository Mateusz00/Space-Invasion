#include "StateStack.hpp"
#include "States/GameState.hpp"
#include "States/MenuState.hpp"
#include "States/PauseState.hpp"
#include <exception>

StateStack::PendingChange::PendingChange(Action action, States::ID id)
    : action(action),
      id(id)
{
}

StateStack::StateStack(State::Context context)
    : mContext(context)
{
    createStateFactory<GameState>(States::GameState);
    createStateFactory<MenuState>(States::MenuState);
    createStateFactory<PauseState>(States::PauseState, false);
    createStateFactory<PauseState>(States::NetworkPause, true);
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
	for(const auto& state : mStack) // Draw states from oldest to newest
		state->draw();
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
        switch(change.action)
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
    mPendingChanges.clear();
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}
