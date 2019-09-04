#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "ResourcesID.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <map>
#include <vector>
class Entity;

namespace NarrowPhase
{
	class AlphaMaskManager
	{
        public:
            sf::Uint8                   getPixel(const std::vector<sf::Uint8>& mask, const sf::Texture*, unsigned int x, unsigned int y) const;
            std::vector<sf::Uint8>&     getMask(const sf::Texture*);
            void                        initializeMasks(const TextureHolder&);

        private:
            void                        createMask(const sf::Texture*);

            std::map<const sf::Texture*, std::vector<sf::Uint8>> mAlphaMasks;
	};

    extern AlphaMaskManager alphaMaskManager;
    bool pixelPerfectTest(const Entity& entity1, const Entity& entity2, sf::Uint8 alphaThreshold);
}

#endif // COLLISION_HPP
