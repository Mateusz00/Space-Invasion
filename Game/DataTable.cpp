#include "DataTable.hpp"
#include "Entities/Aircraft.hpp"

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);

    data[Aircraft::Ally].hitpoints = 100;
    data[Aircraft::Ally].speed = 150.f;
    data[Aircraft::Ally].texture = Textures::PlayerAircraft;
    data[Aircraft::Ally].textureRect = sf::IntRect(73, 0, 73, 42);
    data[Aircraft::Ally].fireInterval = sf::seconds(1.2f);

    data[Aircraft::Enemy].hitpoints = 80;
    data[Aircraft::Enemy].speed = 70.f;
    data[Aircraft::Enemy].texture = Textures::EnemyAircraft;
    //data[Aircraft::Enemy].textureRect = ; <- Add enemy sprites
    data[Aircraft::Enemy].fireInterval = sf::seconds(1.f);

    return data;
}
