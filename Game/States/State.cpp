#include "State.hpp"
#include "../StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
                         SoundPlayer& sounds, MusicPlayer& music, KeyBinding* binds)
    : window(window),
      textures(textures),
      fonts(fonts),
      sounds(sounds),
      music(music),
      keys1(binds)
{
}

State::State(State::Context context, StateStack& stateStack)
    : mContext(context),
      mStateStack(stateStack)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
    mStateStack.pushState(stateID);
}

void State::requestStackPop()
{
	mStateStack.popState();
}

void State::requestStateClear()
{
	mStateStack.clearStates();
}
