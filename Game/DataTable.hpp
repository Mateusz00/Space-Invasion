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

std::vector<AircraftData> initializeAircraftData();

#endif // DATATABLE_HPP
