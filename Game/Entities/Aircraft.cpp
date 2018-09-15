#include "Aircraft.hpp"
#include "HealthBar.hpp"
#include "../Utility.hpp"
#include "../DataTable.hpp"
#include <vector>
namespace
{
    const std::vector<AircraftData> table = initializeAircraftData();
}


Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(table[type].hitpoints),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect),
      mFireRateLevel(1),
      mSpreadLevel(1),
      mMissileAmmo(2),
      mIdentifier(0)
{
    centerOrigin(mSprite);

    // Create HealthBar for aircraft
    sf::FloatRect spriteDimensions = mSprite.getLocalBounds();
    std::unique_ptr<HealthBar> healthBar(new HealthBar(*this, table[mType].hitpoints));
    healthBar->setPosition(0.f, spriteDimensions.height*0.7f);
    attachChild(std::move(healthBar));
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // Add more
    Entity::updateCurrent(dt, commands);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

Category::Type Aircraft::getCategory() const
{
    if(mType == Ally)
        return Category::PlayerAircraft;
    else
        return Category::EnemyAircraft;
}

void Aircraft::increaseFireRate()
{
    mFireRateLevel += 1;
}

void Aircraft::increaseSpread()
{
    mSpreadLevel += 1;
}

int Aircraft::getMissileAmmo() const
{
    return mMissileAmmo;
}

void Aircraft::setMissileAmmo(int missiles)
{
    mMissileAmmo = missiles;
}

int  Aircraft::getIdentifier() const
{
    return mIdentifier;
}

void Aircraft::setIdentifier(int id)
{
    mIdentifier = id;
}

float Aircraft::getMaxSpeed() const
{
    return table[mType].speed;
}

sf::FloatRect Aircraft::getLocalBounds() const
{
    return mSprite.getLocalBounds();
}
