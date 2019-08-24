#include "ProjectileInfo.hpp"
#include "../DataTable.hpp"

namespace ProjectileInfo
{
    const std::unordered_map<int, ProjectileData> table = initializeProjectileData();
}
