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

        spaceshipData.hitpoints      = std::stoi(mainNode.child("hitpoints").text().get());
        spaceshipData.speed          = std::stof(mainNode.child("speed").text().get());
        spaceshipData.textureID      = std::stoi(mainNode.child("textureID").text().get());

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

        data.emplace_back(std::move(spaceshipData));
    }

    return data;
}

std::vector<SpaceshipTextureData> initializeSpaceshipTextureData()
{
    std::vector<SpaceshipTextureData> data(37);

    data[0].texture = Textures::Spaceships; // Player
    data[0].textureRect = sf::IntRect(40, 0, 40, 40);
    data[0].hasRollAnimation = true;
    data[0].spriteNumber = 3;

    data[1].texture = Textures::Spaceships; // HumanShipGreen1
    data[1].textureRect = sf::IntRect(0, 40, 40, 40);

    data[2].texture = Textures::Spaceships; // HumanShipGreen2
    data[2].textureRect = sf::IntRect(40, 40, 40, 40);

    data[3].texture = Textures::Spaceships; // HumanShipGreen3
    data[3].textureRect = sf::IntRect(0, 80, 40, 60);

    data[4].texture = Textures::Spaceships; // HumanShipBlue1
    data[4].textureRect = sf::IntRect(80, 40, 40, 40);

    data[5].texture = Textures::Spaceships; // HumanShipBlue2
    data[5].textureRect = sf::IntRect(120, 40, 40, 40);

    data[6].texture = Textures::Spaceships; // HumanShipBlue3
    data[6].textureRect = sf::IntRect(40, 80, 80, 40);

    data[7].texture = Textures::Spaceships; // BiomechLight1
    data[7].textureRect = sf::IntRect(80, 120, 40, 40);

    data[8].texture = Textures::Spaceships; // BiomechLight2
    data[8].textureRect = sf::IntRect(40, 160, 40, 70);

    data[9].texture = Textures::Spaceships; // BiomechLight3
    data[9].textureRect = sf::IntRect(120, 160, 80, 80);

    data[10].texture = Textures::Spaceships; // BiomechRed1
    data[10].textureRect = sf::IntRect(40, 120, 40, 40);

    data[11].texture = Textures::Spaceships; // BiomechRed2
    data[11].textureRect = sf::IntRect(80, 160, 40, 70);

    data[12].texture = Textures::Spaceships; // BiomechRed3
    data[12].textureRect = sf::IntRect(200, 160, 80, 80);

    data[13].texture = Textures::Spaceships; // BiomechDark1
    data[13].textureRect = sf::IntRect(200, 120, 40, 40);

    data[14].texture = Textures::Spaceships; // BiomechDark2
    data[14].textureRect = sf::IntRect(0, 160, 40, 70);

    data[15].texture = Textures::Spaceships; // BiomechDark3
    data[15].textureRect = sf::IntRect(0, 240, 80, 80);

    data[16].texture = Textures::Spaceships; // SaucerSmall
    data[16].textureRect = sf::IntRect(80, 240, 40, 40);

    data[17].texture = Textures::Spaceships; // SaucerMedium
    data[17].textureRect = sf::IntRect(0, 320, 80, 80);

    data[18].texture = Textures::Spaceships; // SaucerBig
    data[18].textureRect = sf::IntRect(200, 240, 80, 80);

    data[19].texture = Textures::Spaceships; // Mech1
    data[19].textureRect = sf::IntRect(120, 0, 40, 40);

    data[20].texture = Textures::Spaceships; // Mech2
    data[20].textureRect = sf::IntRect(240, 0, 40, 40);

    data[21].texture = Textures::Spaceships; // Mech3
    data[21].textureRect = sf::IntRect(240, 40, 40, 40);

    data[22].texture = Textures::Spaceships; // Mech4
    data[22].textureRect = sf::IntRect(120, 80, 80, 80);

    data[23].texture = Textures::Spaceships; // Mech5
    data[23].textureRect = sf::IntRect(160, 0, 80, 80);

    data[25].texture = Textures::Spaceships; // VoidRed1
    data[25].textureRect = sf::IntRect(80, 280, 25, 40);

    data[26].texture = Textures::Spaceships; // VoidRed2
    data[26].textureRect = sf::IntRect(280, 0, 40, 60);

    data[27].texture = Textures::Spaceships; // VoidRed3
    data[27].textureRect = sf::IntRect(280, 70, 40, 52);

    data[28].texture = Textures::Spaceships; // VoidRed4
    data[28].textureRect = sf::IntRect(280, 120, 48, 80);

    data[29].texture = Textures::Spaceships; // VoidPurple
    data[29].textureRect = sf::IntRect(280, 202, 80, 29);

    data[30].texture = Textures::Spaceships; // VoidBlue1
    data[30].textureRect = sf::IntRect(372, 200, 32, 62);

    data[31].texture = Textures::Spaceships; // VoidBlue2
    data[31].textureRect = sf::IntRect(372, 263, 32, 54);

    data[32].texture = Textures::Spaceships; // PurpleShipFast
    data[32].textureRect = sf::IntRect(440, 0, 80, 80);

    data[33].texture = Textures::Spaceships; // PurpleShipOval
    data[33].textureRect = sf::IntRect(440, 80, 40, 40);

    data[34].texture = Textures::Spaceships; // PurpleShipLong
    data[34].textureRect = sf::IntRect(320, 320, 40, 80);

    data[35].texture = Textures::Spaceships; // PurpleShipWide
    data[35].textureRect = sf::IntRect(360, 320, 120, 40);

    data[36].texture = Textures::Spaceships; // PurpleShipOval
    data[36].textureRect = sf::IntRect(240, 120, 40, 40);

    data[37].texture = Textures::Spaceships; // Boss
    data[37].textureRect = sf::IntRect(520, 0, 360, 360);

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].damage = 20.f;
    data[Projectile::AlliedBullet].texture = Textures::Projectiles;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect(13, 0, 4, 15);

    data[Projectile::EnemyBullet].damage = 20.f;
    data[Projectile::EnemyBullet].texture = Textures::Projectiles;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect(13, 15, 4, 15);

    data[Projectile::Missile].damage = 100.f;
    data[Projectile::Missile].texture = Textures::Projectiles;
    data[Projectile::Missile].textureRect = sf::IntRect(0, 0, 13, 30);

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
        xml_node projectiles        = mainNode.child("projectiles");
        xml_node gravityCenters     = mainNode.child("gravityCenters");

        attackData.chargingTime     = sf::seconds(std::stof(mainNode.child("chargetime").text().get()));
        attackData.cooldown         = sf::seconds(std::stof(mainNode.child("cooldown").text().get()));
        attackData.repeatCooldown   = sf::seconds(repeatNode.attribute("cooldown").as_float());
        attackData.repeats          = repeatNode.attribute("times").as_int();

        // Load info about each projectile of an attack
        for(xml_node projectile : projectiles.children())
        {
            AttackData::ProjectileInfo projectileInfo;

            std::string offsetStr(projectile.attribute("offsets").as_string("0 0"));
            std::string directionStr(projectile.attribute("direction").as_string("0 0"));
            std::string::size_type index;

            if(offsetStr.empty())
                offsetStr = "0 0";
            if(directionStr.empty())
                directionStr = "0 0";

            projectileInfo.offset.x      = std::stof(offsetStr, &index);
            projectileInfo.offset.y      = std::stof(offsetStr.substr(index));
            projectileInfo.direction.x   = std::stof(directionStr, &index);
            projectileInfo.direction.y   = std::stof(directionStr.substr(index));
            projectileInfo.speed         = projectile.attribute("speed").as_float();
            projectileInfo.type          = static_cast<Projectile::Type>(projectile.attribute("id").as_int());
            projectileInfo.pattern       = static_cast<AttackPattern::ID>(projectile.attribute("patternID").as_int());
            projectileInfo.isAimed       = projectile.attribute("aimed").as_bool(); ///Returns def value if null handle check if it's possible to omit unnecessary attributes

            switch(projectileInfo.pattern)
            {
                case AttackPattern::ID::Orbiting:
                    projectileInfo.patternData.gravityCenterID = projectile.attribute("gravityCenterID").as_int(); ///TEST
                    break;

                case AttackPattern::ID::Wave:
                    projectileInfo.patternData.waveData[0] = projectile.attribute("amplitude").as_float();
                    projectileInfo.patternData.waveData[1] = projectile.attribute("waveLength").as_float();

                    if(projectileInfo.offset.x > projectileInfo.patternData.waveData[0])
                        throw std::logic_error("Error(attackID=" + toString(path.first) +
                                               "): Offset.x is higher than amplitude!");
                    break;
            }

            attackData.projectiles.push_back(std::move(projectileInfo));
        }

        // Load gravityCenters
        for(xml_node gravityCenter : gravityCenters.children())
        {
            AttackData::GravityCenterInfo gravityCenterInfo;

            std::string offsetStr(gravityCenter.attribute("offsets").as_string("0 0"));
            std::string directionStr(gravityCenter.attribute("direction").as_string("0 0"));
            std::string::size_type index;

            if(offsetStr.empty())
                offsetStr = "0 0";
            if(directionStr.empty())
                directionStr = "0 0";

            gravityCenterInfo.offset.x      = std::stof(offsetStr, &index);
            gravityCenterInfo.offset.y      = std::stof(offsetStr.substr(index));
            gravityCenterInfo.direction.x   = std::stof(directionStr, &index);
            gravityCenterInfo.direction.y   = std::stof(directionStr.substr(index));
            gravityCenterInfo.speed         = gravityCenter.attribute("speed").as_float();
            gravityCenterInfo.id            = gravityCenter.attribute("id").as_int();
            gravityCenterInfo.pattern       = static_cast<AttackPattern::ID>(gravityCenter.attribute("patternID").as_int());
            gravityCenterInfo.isAimed       = gravityCenter.attribute("aimed").as_bool(); ///Returns def value if null handle check if it's possible to omit unnecessary attributes

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

                    if(gravityCenterInfo.offset.x > gravityCenterInfo.patternData.waveData[0])
                        throw std::logic_error("Error(gravityID=" + toString(gravityCenterInfo.id) + ", attackID="
                                                + toString(path.first) + "): Offset.x is higher than amplitude!");
                    break;
            }

            attackData.gravityCenters.push_back(std::move(gravityCenterInfo));
        }

        data.emplace(path.first, std::move(attackData));
    }

    return data;
}
