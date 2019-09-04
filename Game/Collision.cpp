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

			// Loop through pixels of intersection
			for(int i = intersection.top; i < intersection.top + intersection.height; ++i)
            {
				for(int j = intersection.left; j < intersection.left + intersection.width; ++j)
                {
                    // Reverse position of pixels to match textures
					sf::Vector2f o1v = entity1.getWorldInverseTransform().transformPoint(j, i);
					sf::Vector2f o2v = entity2.getWorldInverseTransform().transformPoint(j, i);

					// Match transformed pixels with texture's pixel positions
                    o1v.x += texRect1.left;
                    o1v.y += texRect1.top;

                    o2v.x += texRect2.left;
                    o2v.y += texRect2.top;

					// Make sure pixels fall within the sprite's subrect
					if(texRect1.contains(sf::Vector2i(o1v)) && texRect2.contains(sf::Vector2i(o2v)))
                    {
                        sf::Uint8 pixel1 = alphaMaskManager.getPixel(mask1, entity1.getTexture(), o1v.x, o1v.y);
                        sf::Uint8 pixel2 = alphaMaskManager.getPixel(mask2, entity2.getTexture(), o2v.x, o2v.y);

                        if(pixel1 > alphaThreshold && pixel2 > alphaThreshold)
                            return true;
					}
				}
			}
		}
		return false;
	}
}
