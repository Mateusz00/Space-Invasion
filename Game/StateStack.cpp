#include "StateStack.hpp"

StateStack::Context::Context(sf::RenderWindow* window, TextureHolder* textures, FontHolder* fonts)
    : window(window),
      textures(textures),
      fonts(fonts)
{
}

StateStack::StateStack(Context context)
    : mContext(context)
{
}
