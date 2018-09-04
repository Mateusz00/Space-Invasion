#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "States/StatesID.hpp"
#include "States/State.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <memory>

class StateStack : private sf::NonCopyable
{
    public:
        explicit StateStack(State::Context);
        void update(sf::Time);
        void draw();
        void handleEvent(const sf::Event&);
        void pushState(States::ID);
        void popState();
        void clearStates();
        void applyPendingChanges();

    private:
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

        std::vector<std::unique_ptr<State>> mStack;
        std::vector<PendingChange> mPendingChanges;
        State::Context mContext;
};

#endif // STATESTACK_HPP
