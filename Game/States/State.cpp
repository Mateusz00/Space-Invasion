#include "State.hpp"
#include "../StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, SoundPlayer& sounds,
                          MusicPlayer& music, std::vector<KeyBinding*>& binds, std::vector<Player>& players)
    : window(window),
      textures(textures),
      fonts(fonts),
      sounds(sounds),
      music(music),
      keys(binds),
      players(players)
{
}

State::State(Context context, StateStack& stateStack)
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

void State::requestStackClear()
{
    mStateStack.clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}
