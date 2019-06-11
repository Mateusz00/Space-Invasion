#ifndef CATEGORY_HPP
#define CATEGORY_HPP

namespace Category
{
    enum Type
    {
        PlayerSpaceship     = 1,
        AirLayer            = 1 << 1,
        EnemySpaceship      = 1 << 2,
        EnemyProjectile     = 1 << 3,
        AlliedProjectile    = 1 << 4,
        Pickup              = 1 << 5,
        Attack              = 1 << 6,
        Collidable          = 1 << 7,
        None                = 0
    };
}

#endif // CATEGORY_HPP
