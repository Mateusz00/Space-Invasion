#ifndef STATE_HPP
#define STATE_HPP

#include "../StateStack.hpp"

class State
{
    public:
        State(StateStack::Context, StateStack*);

    private:
        StateStack::Context     mContext;
        StateStack*             mStateStack;
};

#endif // STATE_HPP
