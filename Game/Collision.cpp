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

		    // Loop through pixels of intersection
            for(float y = intersection.top; y < intersection.top + intersection.height; ++y)
            {
				for(float x = intersection.left; x < intersection.left + intersection.width; ++x)
                {
                    sf::Vector2f p1 = t1.transformPoint(x, y);
                    sf::Vector2f p2 = t2.transformPoint(x, y);

                    p1.x += texRect1.left;
                    p2.x += texRect2.left;
                    p1.y += texRect1.top;
                    p2.y += texRect2.top;

					// Make sure pixels fall within the sprite's subrect
					if(texRect1.contains(sf::Vector2i(p1.x, p1.y)) && texRect2.contains(sf::Vector2i(p2.x, p2.y)))
                    {
                        sf::Uint8 pixel1 = alphaMaskManager.getPixel(mask1, entity1.getTexture(), p1.x, p1.y);
                        sf::Uint8 pixel2 = alphaMaskManager.getPixel(mask2, entity2.getTexture(), p2.x, p2.y);

                        if(pixel1 > alphaThreshold && pixel2 > alphaThreshold)
                            return true;
					}
				}
			}

		    return false;
	    }
    }
}
