#ifndef RESOURCES_ID_HPP
#define RESOURCES_ID_HPP

#include <SFML/Graphics/Rect.hpp>
#include "ResourceHolder.hpp"
namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}

namespace Textures
{
    const sf::IntRect MissileRect = sf::IntRect(0, 0, 13, 30);

    enum ID
    {
        Background,
        Aircrafts,
        Projectiles,
        Pickups,
        Particle,
        Explosion,
        TitleScreen,
        MenuButtons,
        Speaker,
        MutedSpeaker
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

namespace Sound
{
	enum ID
	{
		Explosion,
		GameOver,
		EnemyGun,
		AllyGun,
		Missile,
		PickUp,
		ButtonClick
	};
}

namespace Music
{
    enum ID
    {
		MenuTheme,
		BattleTheme
    };
}

using TextureHolder     = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder        = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, Sound::ID>;

#endif // RESOURCES_ID_HPP
