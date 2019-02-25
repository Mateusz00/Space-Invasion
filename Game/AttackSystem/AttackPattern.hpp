#ifndef ATTACKPATTERN_HPP
#define ATTACKPATTERN_HPP

namespace AttackPattern
{
    enum ID
    {
        StraightLine,
        Guided,
        Spiral,
        Orbiting,
        Barrier
    };

    union PatternData
    {
        int     gravityCenterID;
        float   maxDeviation;
    };
}


#endif // ATTACKPATTERN_HPP
