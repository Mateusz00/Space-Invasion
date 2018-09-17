#include "DataTable.hpp"
#include "Entities/Aircraft.hpp"
#include "Entities/Projectile.hpp"

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);

    data[Aircraft::Ally].hitpoints = 100;
    data[Aircraft::Ally].speed = 150.f;
    data[Aircraft::Ally].texture = Textures::PlayerAircraft;
    data[Aircraft::Ally].textureRect = sf::IntRect(150, 0, 75, 42);
    data[Aircraft::Ally].fireInterval = sf::seconds(1.2f);
    data[Aircraft::Ally].hasRollAnimation = true;
    data[Aircraft::Ally].spriteNumber = 5;

    data[Aircraft::Enemy].hitpoints = 80;
    data[Aircraft::Enemy].speed = 70.f;
    data[Aircraft::Enemy].texture = Textures::EnemyAircraft;
    //data[Aircraft::Enemy].textureRect = ; <- Add enemy sprites
    data[Aircraft::Enemy].fireInterval = sf::seconds(1.f);
    data[Aircraft::Enemy].hasRollAnimation = false;
    data[Aircraft::Enemy].spriteNumber = 0;

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].speed = 250.f;
    data[Projectile::AlliedBullet].damage = 20.f;
    data[Projectile::AlliedBullet].texture = Textures::Projectiles;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect(13, 0, 4, 15);

    data[Projectile::EnemyBullet].speed = 250.f;
    data[Projectile::EnemyBullet].damage = 20.f;
    data[Projectile::EnemyBullet].texture = Textures::Projectiles;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect(13, 15, 4, 15);

    data[Projectile::Missile].speed = 120.f;
    data[Projectile::Missile].damage = 100.f;
    data[Projectile::Missile].texture = Textures::Projectiles;
    data[Projectile::Missile].textureRect = Textures::MissileRect;

    return data;
}
