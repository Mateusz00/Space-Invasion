#ifndef RESOURCES_ID_HPP
#define RESOURCES_ID_HPP

#include "ResourceHolder.hpp"
namespace sf
{
	class Texture;
	class Font;
}


namespace Textures
{
    enum ID
    {
        Background,
        PlayerAircraft,
        EnemyAircraft,
        Bullet,
        Missile,
    };
}

namespace Fonts
{
    enum ID
    {
        Sansation
    };
}

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder    = ResourceHolder<sf::Font, Fonts::ID>;

#endif // RESOURCES_ID_HPP
