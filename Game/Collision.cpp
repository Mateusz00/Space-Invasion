#include "Collision.hpp"
#include "ResourcesID.hpp"
#include "Entities/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <map>

namespace NarrowPhase
{
    AlphaMaskManager alphaMaskManager;

    sf::Uint8 AlphaMaskManager::getPixel(const std::vector<sf::Uint8>& mask, const sf::Texture* tex, unsigned int x, unsigned int y) const
    {
        if(x > tex->getSize().x || y > tex->getSize().y)
            return 0;

        return mask[x + y * tex->getSize().x];
    }

    std::vector<sf::Uint8>& AlphaMaskManager::getMask(const sf::Texture* texture)
    {
        return mAlphaMasks.at(texture);
    }

    void AlphaMaskManager::initializeMasks(const TextureHolder& textures)
    {
        createMask(&textures.get(Textures::Spaceships));
        createMask(&textures.get(Textures::Projectiles));
        createMask(&textures.get(Textures::Pickups));
    }

    void AlphaMaskManager::createMask(const sf::Texture* texture)
    {
        sf::Image img = texture->copyToImage();
        std::vector<sf::Uint8> mask(texture->getSize().y * texture->getSize().x);

        // Fill mask
        for(int y = 0; y < texture->getSize().y; ++y)
        {
            for(int x = 0; x < texture->getSize().x; ++x)
                mask[x + y * texture->getSize().x] = img.getPixel(x, y).a;
        }

        mAlphaMasks.emplace(texture, std::move(mask));
    }

	bool pixelPerfectTest(const Entity& entity1, const Entity& entity2, sf::Uint8 alphaThreshold)
	{
		sf::FloatRect intersection;
		if(entity1.getBoundingRect().intersects(entity2.getBoundingRect(), intersection))
        {
			sf::IntRect texRect1 = entity1.getTextureRect();
			sf::IntRect texRect2 = entity2.getTextureRect();

			std::vector<sf::Uint8>& mask1 = alphaMaskManager.getMask(entity1.getTexture());
			std::vector<sf::Uint8>& mask2 = alphaMaskManager.getMask(entity2.getTexture());

			// Apply reversed transforms to intersection and add texture top-left corners to match
			// texture rects that intersects with each other
            sf::Transform t1 = entity1.getSpriteInverseTransform() * entity1.getWorldInverseTransform();
            sf::Transform t2 = entity2.getSpriteInverseTransform() * entity2.getWorldInverseTransform();

            sf::FloatRect intersection1 = t1.transformRect(intersection);
            sf::FloatRect intersection2 = t2.transformRect(intersection);

            intersection1.left += texRect1.left;
            intersection2.left += texRect2.left;
            intersection1.top += texRect1.top;
            intersection2.top += texRect2.top;

			// Loop through pixels of intersection
			for(int y1 = intersection1.top, y2 = intersection2.top; y1 < intersection1.top + intersection1.height; ++y1, ++y2)
            {
				for(int x1 = intersection1.left, x2 = intersection2.left; x1 < intersection1.left + intersection1.width; ++x1, ++x2)
                {
					// Make sure pixels fall within the sprite's subrect
					if(texRect1.contains(sf::Vector2i(x1, y1)) && texRect2.contains(sf::Vector2i(x2, y2)))
                    {
                        sf::Uint8 pixel1 = alphaMaskManager.getPixel(mask1, entity1.getTexture(), x1, y1);
                        sf::Uint8 pixel2 = alphaMaskManager.getPixel(mask2, entity2.getTexture(), x2, y2);

                        if(pixel1 > alphaThreshold && pixel2 > alphaThreshold)
                            return true;
					}
				}
			}
		}
		return false;
	}
}
