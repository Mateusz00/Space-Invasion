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
    const sf::IntRect MissileRect = sf::IntRect(128, 50, 13, 30);

    enum ID
    {
        Background,
        Spaceships,
        Projectiles,
        Pickups,
        Particle,
        Explosion,
        TitleScreen,
        SettingsButtons,
        Speaker,
        Checkbox,
        FinishLine,
        LevelButtons,
        Checkmark,
        Lock,
        BossDeath,
    };
}

namespace Animation
{
    enum ID
    {
        Explosion,
        VoidPurpleSpaceship,
        VoidBlueSpaceship1,
        VoidBlueSpaceship2,
        VoidRedSpaceship1,
        VoidRedSpaceship2,
        VoidRedSpaceship3,
        VoidRedSpaceship4,
        SaucerSmallSpaceship,
        SaucerMediumSpaceship,
        BossDeathAnimation,
        Count,
    };
}

namespace Fonts
{
    enum ID
    {
        Sansation,
        BPmonoItalics,
        Goodfish
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

namespace Projectiles
{
    enum ID
    {
        GreenOval1 = 0,
        OrangeOval1 = 1,
        GreenBullet = 2,
        BlueOval1 = 3,
        RedCircleSmall = 4,
        RedCircleBig = 5,
        BlueCircleBig = 6,
        BlueStarSmall = 7,
        BlueLaser = 8,
        VioletLaser = 11,
        BigLaser = 14,
        RedSpin = 17,
        OrangeSpin = 18,
        GreenOrb = 19,
        PinkStar = 20,
        BlueBullet = 21,
        OrangeOval2 = 22,
        RedOval = 23,
        BlueStarBig = 24,
        EnergyBallSmall = 25,
        Missile = 26,
        RedLaser = 27,
        BlueOval2 = 30,
        GreenOval2 = 31,
        RedBullet = 32,
        EneryBallBig = 33,
        OrangeOvalWeak1 = 34,
        OrangeOvalWeak2 = 35,
        Count
    };
}

using TextureHolder     = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder        = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, Sound::ID>;

#endif // RESOURCES_ID_HPP
