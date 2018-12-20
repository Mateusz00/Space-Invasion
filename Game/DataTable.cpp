#include "DataTable.hpp"
#include "AnimationNode.hpp"
#include "Entities/Aircraft.hpp"
#include "Entities/Projectile.hpp"
#include "Entities/Pickup.hpp"
#include "ParticleSystem/Particle.hpp"
#include "Exceptions/XMLParseExcepetion.hpp"
#include <SFML/System/Vector2.hpp>
#include <pugixml.hpp>
#include <sstream>

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);

    data[Aircraft::Ally].hitpoints = 100;
    data[Aircraft::Ally].speed = 150.f;
    data[Aircraft::Ally].texture = Textures::Aircrafts;
    data[Aircraft::Ally].textureRect = sf::IntRect(150, 0, 75, 42);
    data[Aircraft::Ally].fireInterval = sf::seconds(1.2f);
    data[Aircraft::Ally].hasRollAnimation = true;
    data[Aircraft::Ally].spriteNumber = 5;

    data[Aircraft::Enemy].hitpoints = 50;
    data[Aircraft::Enemy].speed = 140.f;
    data[Aircraft::Enemy].texture = Textures::Aircrafts;
    data[Aircraft::Enemy].textureRect = sf::IntRect(0, 43, 60, 42);
    data[Aircraft::Enemy].fireInterval = sf::seconds(2.f);
    data[Aircraft::Enemy].hasRollAnimation = false;
    data[Aircraft::Enemy].spriteNumber = 0;
    data[Aircraft::Enemy].directions.push_back(AircraftData::Direction{ 85.f, 80.f});
    data[Aircraft::Enemy].directions.push_back(AircraftData::Direction{-85.f, 80.f});
    data[Aircraft::Enemy].directions.push_back(AircraftData::Direction{ 85.f, 150.f});
    data[Aircraft::Enemy].directions.push_back(AircraftData::Direction{ 20.f, 20.f});
    data[Aircraft::Enemy].directions.push_back(AircraftData::Direction{-70.f, 150.f});
    data[Aircraft::Enemy].directions.push_back(AircraftData::Direction{ 10.f, 20.f});

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
    data[Projectile::Missile].textureRect = sf::IntRect(0, 0, 13, 30);

    return data;
}

std::vector<PickupData> initializePickupData()
{
    using namespace std::placeholders;

    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::FireSpread].action = std::bind(&Aircraft::increaseSpread, _1);
    data[Pickup::FireSpread].texture = Textures::Pickups;
    data[Pickup::FireSpread].textureRect = sf::IntRect(80, 0, 40, 40);

    data[Pickup::FireRate].action = std::bind(&Aircraft::increaseFireRate, _1);
    data[Pickup::FireRate].texture = Textures::Pickups;
    data[Pickup::FireRate].textureRect = sf::IntRect(120, 0, 40, 40);

    data[Pickup::HealthRefill].action = std::bind(&Aircraft::repair, _1, 30);
    data[Pickup::HealthRefill].texture = Textures::Pickups;
    data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 0, 40, 40);

    data[Pickup::MissileRefill].action = std::bind(&Aircraft::changeMissileAmmo, _1, 1);
    data[Pickup::MissileRefill].texture = Textures::Pickups;
    data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 0, 40, 40);

    return data;
}

std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Propellant].lifespan = sf::seconds(0.3f);
    data[Particle::Propellant].color = sf::Color(247, 204, 12);

    data[Particle::Smoke].lifespan = sf::seconds(2.6f);
    data[Particle::Smoke].color = sf::Color(48, 48, 48);

    return data;
}

std::vector<AnimationData> initializeAnimationData()
{
    std::vector<AnimationData> data(AnimationNode::Count);

    data[AnimationNode::Explosion].rows = 4;
    data[AnimationNode::Explosion].framesPerRow = 6;
    data[AnimationNode::Explosion].frames = 24;
    data[AnimationNode::Explosion].frameSize = sf::Vector2i(100, 100);
    data[AnimationNode::Explosion].spriteSheet = Textures::Explosion;

    return data;
}

std::vector<ButtonData> initializeButtonData()
{
    std::vector<ButtonData> data(GUIButton::Count);

    data[GUIButton::ControlsButton].buttonType = GUIButton::Textured;
    data[GUIButton::ControlsButton].textureId = Textures::SettingsButtons;
    data[GUIButton::ControlsButton].buttonSize = sf::Vector2i(203, 50);

    data[GUIButton::TextButton].buttonType = GUIButton::Text;

    data[GUIButton::OptionsButton].buttonType = GUIButton::SimpleRect;
    data[GUIButton::OptionsButton].activatedColor = sf::Color::Black;
    data[GUIButton::OptionsButton].defaultColor = sf::Color(47, 47, 48);

    data[GUIButton::LevelButton].buttonType = GUIButton::Textured;
    data[GUIButton::LevelButton].textureId = Textures::LevelButtons;
    ///data[GUIButton::LevelButton].buttonSize = sf::Vector2i(, ); TODO: ADD texture

    return data;
}

std::vector<LevelData> initializeLevelData()
{
    using namespace pugi;
    std::vector<LevelData> data;

    // Load xml file, throw errors
    xml_document doc;
    xml_parse_result result = doc.load_file("Levels/levels.xml");
    if(!result)
        throw XMLParseExcepetion(result, "levels.xml");

    // Load values and save in LevelData struct
    xml_node levels = doc.child("levels");
    for(xml_node levelInfo : levels.children())
    {
        std::unique_ptr<LevelData> levelData(new LevelData());

        std::istringstream unlocksValues(levelInfo.attribute("unlocks").as_string());
        while(unlocksValues.good())
        {
            int id;
            unlocksValues >> id;
            levelData->levelDependencies.push_back(id);
        }

        levelData->name = levelInfo.attribute("name").as_string();
        levelData->x = levelInfo.attribute("x").as_float();
        levelData->y = levelInfo.attribute("y").as_float();

        data.push_back(std::move(*levelData));
    }

    return data;
}
