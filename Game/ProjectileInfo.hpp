#ifndef PROJECTILEINFO_HPP
#define PROJECTILEINFO_HPP

#include <unordered_map>
class ProjectileData;

namespace ProjectileInfo
{
    extern const std::unordered_map<int, ProjectileData> table;
}

#endif // PROJECTILEINFO_HPP
