#ifndef RESOURCES_ID_HPP
#define RESOURCES_ID_HPP

#include <SFML/Graphics/Rect.hpp>
#include "ResourceHolder.hpp"
namespace sf
{
	class Texture;
	class Font;
}


namespace Textures
{
    static const sf::IntRect MissileRect = sf::IntRect(0, 0, 13, 30);

    enum ID
    {
        Background,
        PlayerAircraft,
        EnemyAircraft,
        Projectiles,
    };
}

namespace Fonts
{
    enum ID
    {
        Sansation,
        BPmonoItalics
    };
}

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder    = ResourceHolder<sf::Font, Fonts::ID>;

#endif // RESOURCES_ID_HPP
