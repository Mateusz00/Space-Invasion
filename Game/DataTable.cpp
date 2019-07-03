#include "DataTable.hpp"
#include "AnimationNode.hpp"
#include "Utility.hpp"
#include "Entities/Spaceship.hpp"
#include "Entities/Projectile.hpp"
#include "Entities/Pickup.hpp"
#include "ParticleSystem/Particle.hpp"
#include "Exceptions/XMLParseException.hpp"
#include <SFML/System/Vector2.hpp>
#include <pugixml.hpp>
#include <sstream>
using namespace pugi;
void resolvePhaseLinking(int attackID, std::unordered_map<int, AttackData>& data);

std::vector<SpaceshipData> initializeSpaceshipData()
{
    std::vector<SpaceshipData> data;
    std::map<int, std::string> pathsMap;

    // Get paths for spaceship xml files
    xml_document pathsDoc;
    xml_parse_result result = pathsDoc.load_file("Spaceships/spaceships.xml");
    if(!result)
        throw XMLParseException(result, "spaceships.xml");

    xml_node paths = pathsDoc.child("spaceships");
    for(xml_node path : paths.children())
        pathsMap.emplace(path.attribute("id").as_int(), (std::string("Spaceships/") + path.attribute("file").as_string()));

    for(const auto& path : pathsMap)
    {
        // Load file
        xml_document spaceshipDoc;
        xml_parse_result result = spaceshipDoc.load_file(path.second.c_str());
        if(!result)
            throw XMLParseException(result, path.second);

        // Load all values
        SpaceshipData spaceshipData;

        xml_node mainNode           = spaceshipDoc.child("spaceship");
        xml_node directionsNode     = mainNode.child("directions");
        xml_node attacksNode        = mainNode.child("attacks");
        xml_node onDeathNode        = mainNode.child("onDeath");

        spaceshipData.hitpoints      = mainNode.child("hitpoints").text().as_int();
        spaceshipData.speed          = mainNode.child("speed").text().as_float();
        spaceshipData.textureID      = mainNode.child("textureID").text().as_int(-1);
        spaceshipData.animationID    = mainNode.child("animationID").text().as_int(-1);

        if(spaceshipData.textureID < 0 && spaceshipData.animationID < 0)
            throw std::runtime_error("XMLError: Couldn't find textureID/animationID or values are negative! " + path.second);

        // Load movement pattern of enemy
        for(xml_node direction : directionsNode.children())
        {
            SpaceshipData::Direction directionData;

            directionData.angle             = direction.attribute("angle").as_float();
            directionData.distance          = direction.attribute("distance").as_float();
            directionData.speedPercentage   = direction.attribute("speedPercentage").as_float();

            spaceshipData.directions.emplace_back(std::move(directionData));
        }

        // Load attacks of that spaceship
        for(xml_node attack : attacksNode.children())
        {
            int attackID        = attack.attribute("attackID").as_int();
            int probability     = attack.attribute("probability").as_int();

            spaceshipData.attacks.emplace(attackID, probability);
        }

        // Load events
        for(xml_node event : onDeathNode.children())
        {
            spaceshipData.eventScheme.eventID = event.attribute("id").as_int(-1);
            event.remove_attribute("id");

            if(spaceshipData.eventScheme.eventID < 0)
                throw std::runtime_error("Event ID not found or less than zero! (" + path.second + ")");

            for(xml_attribute eventAttribute : event.attributes())
                spaceshipData.eventScheme.parameters.emplace(eventAttribute.name(), eventAttribute.value());
        }

        data.emplace_back(std::move(spaceshipData));
    }

    return data;
}

std::vector<SpaceshipTextureData> initializeSpaceshipTextureData()
{
    std::vector<SpaceshipTextureData> data(28);

    data[0].texture = Textures::Spaceships; // Player
    data[0].textureRect = sf::IntRect(40, 0, 40, 40);
    data[0].spriteNumber = 3;

    data[1].texture = Textures::Spaceships; // HumanShipGreen1
    data[1].textureRect = sf::IntRect(0, 40, 40, 40);

    data[2].texture = Textures::Spaceships; // HumanShipGreen2
    data[2].textureRect = sf::IntRect(40, 40, 40, 40);

    data[3].texture = Textures::Spaceships; // HumanShipGreen3
    data[3].textureRect = sf::IntRect(0, 80, 40, 55);

    data[4].texture = Textures::Spaceships; // HumanShipBlue1
    data[4].textureRect = sf::IntRect(88, 49, 24, 25);

    data[5].texture = Textures::Spaceships; // HumanShipBlue2
    data[5].textureRect = sf::IntRect(120, 40, 40, 40);

    data[6].texture = Textures::Spaceships; // HumanShipBlue3
    data[6].textureRect = sf::IntRect(56, 80, 47, 40);

    data[7].texture = Textures::Spaceships; // BiomechLight1
    data[7].textureRect = sf::IntRect(80, 125, 40, 29);

    data[8].texture = Textures::Spaceships; // BiomechLight2
    data[8].textureRect = sf::IntRect(40, 166, 40, 60);

    data[9].texture = Textures::Spaceships; // BiomechLight3
    data[9].textureRect = sf::IntRect(130, 160, 60, 80);

    data[10].texture = Textures::Spaceships; // BiomechRed1
    data[10].textureRect = sf::IntRect(40, 125, 40, 32);

    data[11].texture = Textures::Spaceships; // BiomechRed2
    data[11].textureRect = sf::IntRect(80, 170, 40, 51);

    data[12].texture = Textures::Spaceships; // BiomechRed3
    data[12].textureRect = sf::IntRect(204, 170, 68, 56);

    data[13].texture = Textures::Spaceships; // BiomechDark1
    data[13].textureRect = sf::IntRect(200, 124, 40, 31);

    data[14].texture = Textures::Spaceships; // BiomechDark2
    data[14].textureRect = sf::IntRect(0, 170, 40, 48);

    data[15].texture = Textures::Spaceships; // BiomechDark3
    data[15].textureRect = sf::IntRect(0, 240, 80, 80);

    data[16].texture = Textures::Spaceships; // SaucerBig
    data[16].textureRect = sf::IntRect(200, 240, 80, 80);

    data[17].texture = Textures::Spaceships; // Mech1
    data[17].textureRect = sf::IntRect(120, 0, 40, 40);

    data[18].texture = Textures::Spaceships; // Mech2
    data[18].textureRect = sf::IntRect(240, 0, 40, 40);

    data[19].texture = Textures::Spaceships; // Mech3
    data[19].textureRect = sf::IntRect(240, 40, 40, 40);

    data[20].texture = Textures::Spaceships; // Mech4
    data[20].textureRect = sf::IntRect(120, 80, 80, 80);

    data[21].texture = Textures::Spaceships; // Mech5
    data[21].textureRect = sf::IntRect(160, 0, 80, 80);

    data[22].texture = Textures::Spaceships; // PurpleShipFast
    data[22].textureRect = sf::IntRect(452, 13, 52, 56);

    data[23].texture = Textures::Spaceships; // PurpleShipOval
    data[23].textureRect = sf::IntRect(440, 80, 40, 40);

    data[24].texture = Textures::Spaceships; // PurpleShipLong
    data[24].textureRect = sf::IntRect(320, 320, 40, 80);

    data[25].texture = Textures::Spaceships; // PurpleShipWide
    data[25].textureRect = sf::IntRect(372, 320, 94, 40);

    data[26].texture = Textures::Spaceships; // PurpleShipSmall
    data[26].textureRect = sf::IntRect(240, 120, 40, 40);

    data[27].texture = Textures::Spaceships; // Boss
    data[27].textureRect = sf::IntRect(530, 0, 350, 360);

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectiles::Count);

    data[Projectiles::GreenOval1].damage = 10.f;
    data[Projectiles::GreenOval1].texture = Textures::Projectiles;
    data[Projectiles::GreenOval1].textureRect = sf::IntRect(0, 0, 12, 20);

    data[Projectiles::OrangeOval1].damage = 20.f;
    data[Projectiles::OrangeOval1].texture = Textures::Projectiles;
    data[Projectiles::OrangeOval1].textureRect = sf::IntRect(12, 0, 12, 20);

    data[Projectiles::GreenBullet].damage = 10.f;
    data[Projectiles::GreenBullet].texture = Textures::Projectiles;
    data[Projectiles::GreenBullet].textureRect = sf::IntRect(24, 0, 10, 22);

    data[Projectiles::BlueOval1].damage = 15.f;
    data[Projectiles::BlueOval1].texture = Textures::Projectiles;
    data[Projectiles::BlueOval1].textureRect = sf::IntRect(33, 0, 12, 20);

    data[Projectiles::RedCircleSmall].damage = 30.f;
    data[Projectiles::RedCircleSmall].texture = Textures::Projectiles;
    data[Projectiles::RedCircleSmall].textureRect = sf::IntRect(46, 0, 19, 19);

    data[Projectiles::RedCircleBig].hitpoints = 50.f;
    data[Projectiles::RedCircleBig].damage = 1.f;
    data[Projectiles::RedCircleBig].texture = Textures::Projectiles;
    data[Projectiles::RedCircleBig].textureRect = sf::IntRect(66, 0, 34, 33);

    data[Projectiles::BlueCircleBig].damage = 35.f;
    data[Projectiles::BlueCircleBig].texture = Textures::Projectiles;
    data[Projectiles::BlueCircleBig].textureRect = sf::IntRect(101, 0, 27, 29);

    data[Projectiles::BlueStarSmall].damage = 20.f;
    data[Projectiles::BlueStarSmall].texture = Textures::Projectiles;
    data[Projectiles::BlueStarSmall].textureRect = sf::IntRect(129, 0, 25, 27);

    data[Projectiles::BlueLaserHead].damage = 10.f;
    data[Projectiles::BlueLaserHead].texture = Textures::Projectiles;
    data[Projectiles::BlueLaserHead].textureRect = sf::IntRect(157, 0, 11, 12);

    data[Projectiles::BlueLaserBody].damage = 7.f;
    data[Projectiles::BlueLaserBody].texture = Textures::Projectiles;
    data[Projectiles::BlueLaserBody].textureRect = sf::IntRect(157, 14, 11, 12);

    data[Projectiles::BlueLaserTail].damage = 7.f;
    data[Projectiles::BlueLaserTail].texture = Textures::Projectiles;
    data[Projectiles::BlueLaserTail].textureRect = sf::IntRect(157, 28, 11, 10);

    data[Projectiles::VioletLaserHead].damage = 10.f;
    data[Projectiles::VioletLaserHead].texture = Textures::Projectiles;
    data[Projectiles::VioletLaserHead].textureRect = sf::IntRect(176, 4, 8, 7);

    data[Projectiles::VioletLaserBody].damage = 8.f;
    data[Projectiles::VioletLaserBody].texture = Textures::Projectiles;
    data[Projectiles::VioletLaserBody].textureRect = sf::IntRect(176, 13, 8, 8);

    data[Projectiles::VioletLaserTail].damage = 7.f;
    data[Projectiles::VioletLaserTail].texture = Textures::Projectiles;
    data[Projectiles::VioletLaserTail].textureRect = sf::IntRect(176, 23, 8, 8);

    data[Projectiles::BigLaserHead].hitpoints = 9999.f;
    data[Projectiles::BigLaserHead].damage = 2.f;
    data[Projectiles::BigLaserHead].texture = Textures::Projectiles;
    data[Projectiles::BigLaserHead].textureRect = sf::IntRect(200, 0, 40, 40);

    data[Projectiles::BigLaserBody].hitpoints = 9999.f;
    data[Projectiles::BigLaserBody].damage = 0.65f;
    data[Projectiles::BigLaserBody].texture = Textures::Projectiles;
    data[Projectiles::BigLaserBody].textureRect = sf::IntRect(200, 40, 40, 40);

    data[Projectiles::BigLaserTail].hitpoints = 9999.f;
    data[Projectiles::BigLaserTail].damage = 0.65f;
    data[Projectiles::BigLaserTail].texture = Textures::Projectiles;
    data[Projectiles::BigLaserTail].textureRect = sf::IntRect(160, 40, 40, 40);

    data[Projectiles::RedSpin].damage = 20.f;
    data[Projectiles::RedSpin].texture = Textures::Projectiles;
    data[Projectiles::RedSpin].textureRect = sf::IntRect(0, 21, 16, 16);

    data[Projectiles::OrangeSpin].damage = 20.f;
    data[Projectiles::OrangeSpin].texture = Textures::Projectiles;
    data[Projectiles::OrangeSpin].textureRect = sf::IntRect(16, 23, 17, 13);

    data[Projectiles::GreenOrb].damage = 50.f;
    data[Projectiles::GreenOrb].texture = Textures::Projectiles;
    data[Projectiles::GreenOrb].textureRect = sf::IntRect(34, 20, 20, 20);

    data[Projectiles::PinkStar].damage = 20.f;
    data[Projectiles::PinkStar].texture = Textures::Projectiles;
    data[Projectiles::PinkStar].textureRect = sf::IntRect(124, 29, 20, 20);

    data[Projectiles::BlueBullet].damage = 20.f;
    data[Projectiles::BlueBullet].texture = Textures::Projectiles;
    data[Projectiles::BlueBullet].textureRect = sf::IntRect(0, 38, 10, 20);

    data[Projectiles::OrangeOval2].damage = 20.f;
    data[Projectiles::OrangeOval2].texture = Textures::Projectiles;
    data[Projectiles::OrangeOval2].textureRect = sf::IntRect(11, 36, 16, 24);

    data[Projectiles::RedOval].damage = 20.f;
    data[Projectiles::RedOval].texture = Textures::Projectiles;
    data[Projectiles::RedOval].textureRect = sf::IntRect(27, 40, 13, 19);

    data[Projectiles::BlueStarBig].damage = 25.f;
    data[Projectiles::BlueStarBig].texture = Textures::Projectiles;
    data[Projectiles::BlueStarBig].textureRect = sf::IntRect(40, 40, 40, 40);

    data[Projectiles::EnergyBallSmall].hitpoints = 40.f;
    data[Projectiles::EnergyBallSmall].damage = 0.8f;
    data[Projectiles::EnergyBallSmall].texture = Textures::Projectiles;
    data[Projectiles::EnergyBallSmall].textureRect = sf::IntRect(80, 40, 40, 40);

    data[Projectiles::Missile].damage = 100.f;
    data[Projectiles::Missile].texture = Textures::Projectiles;
    data[Projectiles::Missile].textureRect = sf::IntRect(128, 50, 13, 30);

    data[Projectiles::RedLaserHead].damage = 15.f;
    data[Projectiles::RedLaserHead].texture = Textures::Projectiles;
    data[Projectiles::RedLaserHead].textureRect = sf::IntRect(148, 40, 11, 11);

    data[Projectiles::RedLaserBody].damage = 7.f;
    data[Projectiles::RedLaserBody].texture = Textures::Projectiles;
    data[Projectiles::RedLaserBody].textureRect = sf::IntRect(148, 53, 11, 12);

    data[Projectiles::RedLaserTail].damage = 1.f;
    data[Projectiles::RedLaserTail].texture = Textures::Projectiles;
    data[Projectiles::RedLaserTail].textureRect = sf::IntRect(148, 67, 11, 10);

    data[Projectiles::BlueOval2].damage = 20.f;
    data[Projectiles::BlueOval2].texture = Textures::Projectiles;
    data[Projectiles::BlueOval2].textureRect = sf::IntRect(0, 60, 13, 18);

    data[Projectiles::GreenOval2].damage = 20.f;
    data[Projectiles::GreenOval2].texture = Textures::Projectiles;
    data[Projectiles::GreenOval2].textureRect = sf::IntRect(14, 61, 13, 16);

    data[Projectiles::RedBullet].damage = 20.f;
    data[Projectiles::RedBullet].texture = Textures::Projectiles;
    data[Projectiles::RedBullet].textureRect = sf::IntRect(29, 61, 8, 17);

    data[Projectiles::EneryBallBig].hitpoints = 9999.f;
    data[Projectiles::EneryBallBig].damage = 0.42f;
    data[Projectiles::EneryBallBig].texture = Textures::Projectiles;
    data[Projectiles::EneryBallBig].textureRect = sf::IntRect(0, 80, 240, 220);

    data[Projectiles::OrangeOvalWeak1].damage = 10.f;
    data[Projectiles::OrangeOvalWeak1].texture = Textures::Projectiles;
    data[Projectiles::OrangeOvalWeak1].textureRect = sf::IntRect(12, 0, 12, 20);

    data[Projectiles::OrangeOvalWeak2].damage = 6.f;
    data[Projectiles::OrangeOvalWeak2].texture = Textures::Projectiles;
    data[Projectiles::OrangeOvalWeak2].textureRect = sf::IntRect(12, 0, 12, 20);

    return data;
}

std::vector<PickupData> initializePickupData()
{
    using namespace std::placeholders;

    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::FireSpread].action = std::bind(&Spaceship::increaseSpread, _1);
    data[Pickup::FireSpread].texture = Textures::Pickups;
    data[Pickup::FireSpread].textureRect = sf::IntRect(80, 0, 40, 40);

    data[Pickup::FireRate].action = std::bind(&Spaceship::increaseFireRate, _1);
    data[Pickup::FireRate].texture = Textures::Pickups;
    data[Pickup::FireRate].textureRect = sf::IntRect(120, 0, 40, 40);

    data[Pickup::HealthRefill].action = std::bind(&Spaceship::repair, _1, 30);
    data[Pickup::HealthRefill].texture = Textures::Pickups;
    data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 0, 40, 40);

    data[Pickup::MissileRefill].action = std::bind(&Spaceship::changeMissileAmmo, _1, 1);
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
    std::vector<AnimationData> data(Animation::Count);

    data[Animation::Explosion].rows = 4;
    data[Animation::Explosion].framesPerRow = 6;
    data[Animation::Explosion].frames = 24;
    data[Animation::Explosion].frameSize = sf::Vector2i(100, 100);
    data[Animation::Explosion].spriteSheet = Textures::Explosion;

    data[Animation::VoidPurpleSpaceship].beginning = sf::Vector2i(280, 202);
    data[Animation::VoidPurpleSpaceship].rows = 4;
    data[Animation::VoidPurpleSpaceship].framesPerRow = 1;
    data[Animation::VoidPurpleSpaceship].frames = 4;
    data[Animation::VoidPurpleSpaceship].frameSize = sf::Vector2i(80, 29);
    data[Animation::VoidPurpleSpaceship].spriteSheet = Textures::Spaceships;

    data[Animation::VoidBlueSpaceship1].beginning = sf::Vector2i(372, 200);
    data[Animation::VoidBlueSpaceship1].rows = 1;
    data[Animation::VoidBlueSpaceship1].framesPerRow = 4;
    data[Animation::VoidBlueSpaceship1].frames = 4;
    data[Animation::VoidBlueSpaceship1].frameSize = sf::Vector2i(32, 62);
    data[Animation::VoidBlueSpaceship1].spriteSheet = Textures::Spaceships;

    data[Animation::VoidBlueSpaceship2].beginning = sf::Vector2i(372, 263);
    data[Animation::VoidBlueSpaceship2].rows = 1;
    data[Animation::VoidBlueSpaceship2].framesPerRow = 4;
    data[Animation::VoidBlueSpaceship2].frames = 4;
    data[Animation::VoidBlueSpaceship2].frameSize = sf::Vector2i(32, 54);
    data[Animation::VoidBlueSpaceship2].spriteSheet = Textures::Spaceships;

    data[Animation::VoidRedSpaceship1].beginning = sf::Vector2i(80, 280);
    data[Animation::VoidRedSpaceship1].rows = 1;
    data[Animation::VoidRedSpaceship1].framesPerRow = 4;
    data[Animation::VoidRedSpaceship1].frames = 4;
    data[Animation::VoidRedSpaceship1].frameSize = sf::Vector2i(16, 40);
    data[Animation::VoidRedSpaceship1].spriteSheet = Textures::Spaceships;

    data[Animation::VoidRedSpaceship2].beginning = sf::Vector2i(280, 0);
    data[Animation::VoidRedSpaceship2].rows = 1;
    data[Animation::VoidRedSpaceship2].framesPerRow = 4;
    data[Animation::VoidRedSpaceship2].frames = 4;
    data[Animation::VoidRedSpaceship2].frameSize = sf::Vector2i(40, 60);
    data[Animation::VoidRedSpaceship2].spriteSheet = Textures::Spaceships;

    data[Animation::VoidRedSpaceship3].beginning = sf::Vector2i(280, 70);
    data[Animation::VoidRedSpaceship3].rows = 1;
    data[Animation::VoidRedSpaceship3].framesPerRow = 4;
    data[Animation::VoidRedSpaceship3].frames = 4;
    data[Animation::VoidRedSpaceship3].frameSize = sf::Vector2i(40, 51);
    data[Animation::VoidRedSpaceship3].spriteSheet = Textures::Spaceships;

    data[Animation::VoidRedSpaceship4].beginning = sf::Vector2i(280, 120);
    data[Animation::VoidRedSpaceship4].rows = 1;
    data[Animation::VoidRedSpaceship4].framesPerRow = 4;
    data[Animation::VoidRedSpaceship4].frames = 4;
    data[Animation::VoidRedSpaceship4].frameSize = sf::Vector2i(48, 80);
    data[Animation::VoidRedSpaceship4].spriteSheet = Textures::Spaceships;

    data[Animation::SaucerSmallSpaceship].beginning = sf::Vector2i(80, 240);
    data[Animation::SaucerSmallSpaceship].rows = 1;
    data[Animation::SaucerSmallSpaceship].framesPerRow = 3;
    data[Animation::SaucerSmallSpaceship].frames = 3;
    data[Animation::SaucerSmallSpaceship].frameSize = sf::Vector2i(32, 31);
    data[Animation::SaucerSmallSpaceship].spriteSheet = Textures::Spaceships;

    data[Animation::SaucerMediumSpaceship].beginning = sf::Vector2i(0, 320);
    data[Animation::SaucerMediumSpaceship].rows = 1;
    data[Animation::SaucerMediumSpaceship].framesPerRow = 4;
    data[Animation::SaucerMediumSpaceship].frames = 4;
    data[Animation::SaucerMediumSpaceship].frameSize = sf::Vector2i(60, 60);
    data[Animation::SaucerMediumSpaceship].spriteSheet = Textures::Spaceships;

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

    data[GUIButton::ScoresButton].buttonType = GUIButton::SimpleRect;
    data[GUIButton::ScoresButton].activatedColor = sf::Color(105, 17, 7);
    data[GUIButton::ScoresButton].defaultColor = sf::Color(161, 32, 18);

    data[GUIButton::LevelButton].buttonType = GUIButton::Textured;
    data[GUIButton::LevelButton].textureId = Textures::LevelButtons;
    data[GUIButton::LevelButton].buttonSize = sf::Vector2i(50, 50);

    return data;
}

std::vector<LevelData> initializeLevelData()
{
    std::vector<LevelData> data;

    // Load xml file, throw errors
    xml_document doc;
    xml_parse_result result = doc.load_file("Levels/levels.xml");
    if(!result)
        throw XMLParseException(result, "levels.xml");

    // Load values and save in LevelData struct
    xml_node levels = doc.child("levels");
    for(xml_node levelInfo : levels.children())
    {
        std::unique_ptr<LevelData> levelData(new LevelData());

        std::istringstream unlocksValues(levelInfo.attribute("unlocks").as_string());
        while(unlocksValues.good())
        {
            int id = 0;
            unlocksValues >> id;
            if(id != 0)
                levelData->levelDependencies.push_back(id);
        }

        levelData->name = levelInfo.attribute("name").as_string();
        levelData->x = levelInfo.attribute("x").as_float();
        levelData->y = levelInfo.attribute("y").as_float();
        levelData->id = levelInfo.attribute("id").as_int();

        data.push_back(std::move(*levelData));
    }

    return data;
}

std::unordered_map<int, AttackData> initializeAttackData()
{
    std::unordered_map<int, AttackData>  data;
    std::unordered_map<int, std::string> pathsMap;

    // Get paths for attacks files
    xml_document pathsDoc;
    xml_parse_result result = pathsDoc.load_file("Attacks/attacks.xml");
    if(!result)
        throw XMLParseException(result, "attacks.xml");

    xml_node paths = pathsDoc.child("attacks");
    for(xml_node path : paths.children())
        pathsMap.emplace(path.attribute("id").as_int(), (std::string("Attacks/") + path.attribute("file").as_string()));

    // Load attacks from corresponding files
    for(const auto& path : pathsMap)
    {
        // Load file
        xml_document attackDoc;
        xml_parse_result result = attackDoc.load_file(path.second.c_str());
        if(!result)
            throw XMLParseException(result, path.second);

        // Load all values
        AttackData attackData;

        xml_node mainNode           = attackDoc.child("attack");
        xml_node repeatNode         = mainNode.child("repeat");

        attackData.chargingTime     = sf::seconds(mainNode.child("chargetime").text().as_float());
        attackData.cooldown         = sf::seconds(mainNode.child("cooldown").text().as_float());

        for(xml_node phase = mainNode.child("attackPhase"); phase; phase = phase.next_sibling("attackPhase"))
        {
            AttackData::AttackPhase attackPhaseData;

            attackPhaseData.repeats         = phase.attribute("repeats").as_int(1);
            attackPhaseData.repeatCooldown  = sf::seconds(phase.attribute("repeatCooldown").as_float());
            attackPhaseData.phaseCooldown   = sf::seconds(phase.attribute("phaseCooldown").as_float());
            attackPhaseData.linkedAttackID  = phase.attribute("linkedAttackID").as_int(-1);

            // If attack phase is linked to already existing attack then ignore other attack phase data
            if(attackPhaseData.linkedAttackID < 0)
            {
                xml_node projectiles        = phase.child("projectiles");
                xml_node gravityCenters     = phase.child("gravityCenters");

                // Load info about each projectile of an attack
                for(xml_node projectile : projectiles.children())
                {
                    AttackData::ProjectileInfo projectileInfo;

                    std::string offsetStr(projectile.attribute("offsets").as_string("0 0"));
                    std::string::size_type index;

                    if(offsetStr.empty())
                        offsetStr = "0 0";

                    projectileInfo.offset.x      = std::stof(offsetStr, &index);
                    projectileInfo.offset.y      = std::stof(offsetStr.substr(index));
                    projectileInfo.direction     = projectile.attribute("direction").as_float();
                    projectileInfo.speed         = projectile.attribute("speed").as_float();
                    projectileInfo.type          = static_cast<Projectiles::ID>(projectile.attribute("id").as_int());
                    projectileInfo.pattern       = static_cast<AttackPattern::ID>(projectile.attribute("patternID").as_int());
                    projectileInfo.isAimed       = projectile.attribute("aimed").as_bool();

                    switch(projectileInfo.pattern)
                    {
                        case AttackPattern::ID::Orbiting:
                            projectileInfo.patternData.gravityCenterID = projectile.attribute("gravityCenterID").as_int();
                            break;

                        case AttackPattern::ID::Wave:
                            projectileInfo.patternData.waveData[0] = projectile.attribute("amplitude").as_float();
                            projectileInfo.patternData.waveData[1] = projectile.attribute("waveLength").as_float();
                            break;
                    }

                    attackPhaseData.projectiles.push_back(std::move(projectileInfo));
                }

                // Load gravityCenters
                for(xml_node gravityCenter : gravityCenters.children())
                {
                    AttackData::GravityCenterInfo gravityCenterInfo;

                    std::string offsetStr(gravityCenter.attribute("offsets").as_string("0 0"));
                    std::string::size_type index;

                    if(offsetStr.empty())
                        offsetStr = "0 0";

                    gravityCenterInfo.offset.x      = std::stof(offsetStr, &index);
                    gravityCenterInfo.offset.y      = std::stof(offsetStr.substr(index));
                    gravityCenterInfo.direction     = gravityCenter.attribute("direction").as_float();
                    gravityCenterInfo.speed         = gravityCenter.attribute("speed").as_float();
                    gravityCenterInfo.id            = gravityCenter.attribute("id").as_int();
                    gravityCenterInfo.pattern       = static_cast<AttackPattern::ID>(gravityCenter.attribute("patternID").as_int());
                    gravityCenterInfo.isAimed       = gravityCenter.attribute("aimed").as_bool();

                    switch(gravityCenterInfo.pattern)
                    {
                        case AttackPattern::Orbiting:
                            gravityCenterInfo.patternData.gravityCenterID = gravityCenter.attribute("gravityCenterID").as_int();

                            if(gravityCenterInfo.id <= gravityCenterInfo.patternData.gravityCenterID)
                                throw std::logic_error("Wrong gravityCenter id(" + toString(gravityCenterInfo.id) +
                                                       ")in XML file (attackID=" + toString(path.first) + ")");
                            break;

                        case AttackPattern::Wave:
                            gravityCenterInfo.patternData.waveData[0] = gravityCenter.attribute("amplitude").as_float();
                            gravityCenterInfo.patternData.waveData[1] = gravityCenter.attribute("waveLength").as_float();
                            break;
                    }
                    attackPhaseData.gravityCenters.push_back(std::move(gravityCenterInfo));
                }
            }
            attackData.phases.push_back(attackPhaseData);
        }
        data.emplace(path.first, std::move(attackData));
    }

    // Resolve phase linking
    for(const std::pair<int, AttackData>& attack : data)
        resolvePhaseLinking(attack.first, data);

    return data;
}

void resolvePhaseLinking(int attackID, std::unordered_map<int, AttackData>& data)
{
    std::vector<AttackData::PhaseInfo>& phaseQueue = data[attackID].phaseQueue;

    // If phaseQueue has elements then that means links in this attack are resolved
    if(phaseQueue.size() > 0)
        return;

    for(int phase=0; phase < data[attackID].phases.size(); ++phase)
    {
        int linkedID = data[attackID].phases[phase].linkedAttackID;

        if(linkedID < 0)
        {
            phaseQueue.emplace_back(AttackData::PhaseInfo{attackID, phase, data[attackID].phases[phase].phaseCooldown});
        }
        else
        {
            resolvePhaseLinking(linkedID, data);
            for(int i=0; i < data[attackID].phases[phase].repeats; ++i)
            {
                phaseQueue.insert(phaseQueue.end(), data[linkedID].phaseQueue.begin(), data[linkedID].phaseQueue.end());
                phaseQueue.back().phaseCooldown = data[attackID].phases[phase].repeatCooldown; // Extra cool-down between each repeat of a linked attack
            }

            phaseQueue.back().phaseCooldown = data[attackID].phases[phase].phaseCooldown; // Overwrite last phase's phaseCooldown
        }
    }
}
