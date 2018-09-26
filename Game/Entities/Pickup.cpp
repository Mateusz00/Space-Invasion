#include "Pickup.hpp"
#include "../Utility.hpp"
#include "../DataTable.hpp"

namespace
{
	const std::vector<PickupData> table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures)
    : Entity(1),
      mType(type),
      mSprite(textures.get(table[type].texture), table[type].textureRect)
{
	centerOrigin(mSprite);
}

Category::Type Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Aircraft& player) const
{
	table[mType].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
