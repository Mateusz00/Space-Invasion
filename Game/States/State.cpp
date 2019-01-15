#include "State.hpp"
#include "../StateStack.hpp"
#include "../KeyBinding.hpp"
#include "../Settings.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, SoundPlayer& sounds,
                          MusicPlayer& music, std::vector<KeyBinding*>& binds, std::vector<Player>& players,
                          Settings& settings, Profile& profile)
    : window(window),
      textures(textures),
      fonts(fonts),
      sounds(sounds),
      music(music),
      keys(binds),
      players(players),
      settings(settings),
      profile(profile)
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
