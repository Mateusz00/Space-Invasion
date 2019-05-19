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
        GreenOval1,
        OrangeOval1,
        GreenBullet,
        BlueOval1,
        RedCircleSmall,
        RedCircleBig,
        BlueCircleBig,
        BlueStarSmall,
        BlueLaserHead,
        BlueLaserBody,
        BlueLaserTail,
        VioletLaserHead,
        VioletLaserBody,
        VioletLaserTail,
        BigLaserHead,
        BigLaserBody,
        BigLaserTail,
        RedSpin,
        OrangeSpin,
        GreenOrb,
        PinkStar,
        BlueBullet,
        OrangeOval2,
        RedOval,
        BlueStarBig,
        EnergyBallSmall,
        Missile,
        RedLaserHead,
        RedLaserBody,
        RedLaserTail,
        BlueOval2,
        GreenOval2,
        RedBullet,
        EneryBallBig,
        OrangeOvalWeak1,
        OrangeOvalWeak2,
        Count
    };
}

using TextureHolder     = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder        = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, Sound::ID>;

#endif // RESOURCES_ID_HPP
