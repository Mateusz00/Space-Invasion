#include "Attack.hpp"
#include "DataTable.hpp"
namespace
{
    const std::vector<AttackData> attackData = initializeAttackData();
}

Attack::Attack()
{
}

void Attack::pushProjectile(Projectile*)
{
}

void Attack::update(sf::Time)
{
}
