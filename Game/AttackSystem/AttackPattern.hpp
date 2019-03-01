#ifndef ATTACKPATTERN_HPP
#define ATTACKPATTERN_HPP

#include <SFML/System/Vector2.hpp>
#include <array>

namespace AttackPattern
{
    enum ID
    {
        StraightLine,
        Guided,
        Wave,
        Orbiting,
        Barrier
    };

    union PatternData
    {
        int                     gravityCenterID;
        /** 0=amplitude, 1=waveLength*/
        std::array<float, 2>    waveData;
    };
}


#endif // ATTACKPATTERN_HPP
