#ifndef DATATABLE_HPP
#define DATATABLE_HPP

#include "ResourcesID.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

struct AircraftData
{
    int             hitpoints;
    float           speed;
    Textures::ID    texture;
    sf::IntRect     textureRect;
    sf::Time        fireInterval;
    bool            hasRollAnimation;
    int             spriteNumber;
};

struct ProjectileData
{
    float           speed;
    int             damage;
    Textures::ID    texture;
    sf::IntRect     textureRect;
};

std::vector<AircraftData>       initializeAircraftData();
std::vector<ProjectileData>     initializeProjectileData();

#endif // DATATABLE_HPP
