#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "States/StatesID.hpp"
#include "States/State.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <map>
#include <memory>
#include <functional>

class StateStack : private sf::NonCopyable
{
    public:
        explicit    StateStack(State::Context);
        void        update(sf::Time);
        void        draw();
        void        handleEvent(const sf::Event&);
        void        pushState(States::ID);
        void        popState();
        void        clearStates();
        void        applyPendingChanges();
        bool        isEmpty() const;

    private:
        template <typename T>
        void createStateFactory(States::ID);

        template <typename T, typename CtorParam1>
        void createStateFactory(States::ID, CtorParam1);

        enum Action
        {
            Push,
            Pop,
            Clear
        };
        struct PendingChange // We need to perform update on current states first, then we can delete/add them to stack
        {
            explicit    PendingChange(Action, States::ID id = States::None);
            Action      action;
            States::ID  id;
        };

        std::vector<State::Ptr>                             mStack;
        std::vector<PendingChange>                          mPendingChanges;
        State::Context                                      mContext;
        std::map<States::ID, std::function<State::Ptr()>>   mFactory;
};

template <typename T>
void StateStack::createStateFactory(States::ID id)
{
    mFactory[id] = [this]() // Lambda that creates state
    {
        return State::Ptr(new T(mContext, *this));
    };
}

template <typename T, typename CtorParam1>
void StateStack::createStateFactory(States::ID id, CtorParam1 arg1)
{
    mFactory[id] = [this, arg1]() // Lambda that creates state
    {
        return State::Ptr(new T(mContext, *this, arg1));
    };
}

#endif // STATESTACK_HPP
