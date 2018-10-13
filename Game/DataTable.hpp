#ifndef DATATABLE_HPP
#define DATATABLE_HPP

#include "ResourcesID.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
class Aircraft;

struct AircraftData
{
    struct Direction
    {
        float angle;
        float distance;
    };

    int                     hitpoints;
    float                   speed;
    Textures::ID            texture;
    sf::IntRect             textureRect;
    sf::Time                fireInterval;
    bool                    hasRollAnimation;
    int                     spriteNumber;
    std::vector<Direction>  directions;
};

struct ProjectileData
{
    float           speed;
    int             damage;
    Textures::ID    texture;
    sf::IntRect     textureRect;
};

struct PickupData
{
	std::function<void(Aircraft&)>	action;
	Textures::ID					texture;
    sf::IntRect						textureRect;
};

struct ParticleData
{
    sf::Time    lifespan;
    sf::Color   color;
};

struct AnimationData
{
	int             rows;
	int             framesPerRow;
	int             frames;
	sf::Vector2i 	frameSize;
	Textures::ID    spriteSheet;
};

std::vector<AircraftData>       initializeAircraftData();
std::vector<ProjectileData>     initializeProjectileData();
std::vector<PickupData>		    initializePickupData();
std::vector<ParticleData>	    initializeParticleData();
std::vector<AnimationData>      initializeAnimationData();

#endif // DATATABLE_HPP
