#include "State.hpp"

State::State(StateStack::Context context, StateStack* ptr)
    : mContext(context),
      mStateStack(ptr)
{
}
