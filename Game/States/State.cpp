#include "State.hpp"

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
