#include "State.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, KeyBinding* binds)
    : window(window),
      textures(textures),
      fonts(fonts),
      keys1(binds)
{
}

State::State(State::Context context, StateStack& stateStack)
    : mContext(context),
      mStateStack(stateStack)
{
}
