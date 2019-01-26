#ifndef DATATABLE_HPP
#define DATATABLE_HPP

#include "ResourcesID.hpp"
#include "Entities/Projectile.hpp"
#include "GUI/GUIButton.hpp"
#include "Attack.hpp"
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
        float speedPercentage;
    };

    int                             hitpoints;
    float                           speed;
    int                             textureID;
    std::vector<Direction>          directions;
    std::unordered_map<int, int>    attacks; // attackID, probability
};

struct AircraftTextureData
{
    Textures::ID    texture;
    sf::IntRect     textureRect;
    bool            hasRollAnimation;
    int             spriteNumber;
};

struct ProjectileData
{
    int             damage;
    Textures::ID    texture;
    sf::IntRect     textureRect;
};

struct PickupData
{
    std::function<void(Aircraft&)>  action;
    Textures::ID                    texture;
    sf::IntRect                     textureRect;
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
    sf::Vector2i    frameSize;
    Textures::ID    spriteSheet;
};

struct ButtonData
{
    GUIButton::ButtonType   buttonType;
    Textures::ID            textureId;
    sf::Vector2i            buttonSize;
    sf::Color               defaultColor;
    sf::Color               activatedColor;
};

struct LevelData
{
    std::vector<int>   levelDependencies;
    std::string        name;
    float              x;
    float              y;
    int                id;
};

struct AttackData
{
    sf::Time                        chargingTime = sf::Seconds(0.f);
    bool                            attackInPlayerDirection;
    std::vector<float>              projectileSpeeds;
    std::vector<Projectile::Type>   projectileTypes;
    std::vector<sf::Vector2f>       projectileOffsets;
    std::vector<Attack::Behavior>   projectileBehavior;
    sf::Time                        cooldown;
};

std::vector<AircraftData>           initializeAircraftData();
std::vector<AircraftTextureData>    initializeAircraftTextureData();
std::vector<ProjectileData>         initializeProjectileData();
std::vector<PickupData>             initializePickupData();
std::vector<ParticleData>           initializeParticleData();
std::vector<AnimationData>          initializeAnimationData();
std::vector<ButtonData>             initializeButtonData();
std::vector<LevelData>              initializeLevelData();
std::vector<AttackData>             initializeAttackData();

#endif // DATATABLE_HPP
