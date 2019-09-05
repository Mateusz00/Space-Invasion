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
                    sf::Transform t1 = entity1.getSpriteInverseTransform() * entity1.getWorldInverseTransform();
                    sf::Transform t2 = entity2.getSpriteInverseTransform() * entity2.getWorldInverseTransform();
					sf::Vector2f tex1Point = t1.transformPoint(j, i);
					sf::Vector2f tex2Point = t2.transformPoint(j, i);

					// Match transformed pixels with texture's pixel positions
                    tex1Point.x += texRect1.left;
                    tex1Point.y += texRect1.top;

                    tex2Point.x += texRect2.left;
                    tex2Point.y += texRect2.top;

					// Make sure pixels fall within the sprite's subrect
					if(texRect1.contains(sf::Vector2i(tex1Point)) && texRect2.contains(sf::Vector2i(tex2Point)))
                    {
                        sf::Uint8 pixel1 = alphaMaskManager.getPixel(mask1, entity1.getTexture(), tex1Point.x, tex1Point.y);
                        sf::Uint8 pixel2 = alphaMaskManager.getPixel(mask2, entity2.getTexture(), tex2Point.x, tex2Point.y);

                        if(pixel1 > alphaThreshold && pixel2 > alphaThreshold)
                            return true;
					}
				}
			}
		}
		return false;
	}
}
