#ifndef DATATABLE_HPP
#define DATATABLE_HPP

#include "ResourcesID.hpp"
#include "Entities/Projectile.hpp"
#include "GUI/GUIButton.hpp"
#include "AttackSystem/AttackPattern.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <unordered_map>
class Spaceship;

struct SpaceshipData
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

struct SpaceshipTextureData
{
    Textures::ID    texture;
    sf::IntRect     textureRect;
    bool            hasRollAnimation = false;
    int             spriteNumber = 0;
};

struct ProjectileData
{
    int             damage;
    Textures::ID    texture;
    sf::IntRect     textureRect;
};

struct PickupData
{
    std::function<void(Spaceship&)> action;
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
    sf::Vector2i    beginning = sf::Vector2i(0, 0);
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
    struct ProjectileInfo
    {
        float                           speed;
        Projectile::Type                type;
        sf::Vector2f                    offset;
        sf::Vector2f                    direction;
        AttackPattern::ID               pattern;
        AttackPattern::PatternData      patternData;
        bool                            isAimed;
    };
    struct GravityCenterInfo
    {
        float                           speed;
        int                             id;
        sf::Vector2f                    offset;
        sf::Vector2f                    direction;
        AttackPattern::ID               pattern;
        AttackPattern::PatternData      patternData;
        bool                            isAimed;
    };

    sf::Time                            chargingTime;
    bool                                attackInPlayerDirection;
    int                                 repeats;
    sf::Time                            repeatCooldown;
    sf::Time                            cooldown;
    std::vector<ProjectileInfo>         projectiles;
    std::vector<GravityCenterInfo>      gravityCenters;
};

std::vector<SpaceshipData>              initializeSpaceshipData();
std::vector<SpaceshipTextureData>       initializeSpaceshipTextureData();
std::vector<ProjectileData>             initializeProjectileData();
std::vector<PickupData>                 initializePickupData();
std::vector<ParticleData>               initializeParticleData();
std::vector<AnimationData>              initializeAnimationData();
std::vector<ButtonData>                 initializeButtonData();
std::vector<LevelData>                  initializeLevelData();
std::unordered_map<int, AttackData>     initializeAttackData();

#endif // DATATABLE_HPP
