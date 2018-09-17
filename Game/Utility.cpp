#include "Utility.hpp"
#include <cmath>

float vectorLength(sf::Vector2f dist)
{
    return std::sqrt(dist.x * dist.x + dist.y * dist.y);
}

sf::Vector2f oneUnitVector(sf::Vector2f distance)
{
    if(distance == sf::Vector2f(0.f, 0.f))
        throw std::logic_error("oneUnitVector: Wrong length of vector!");

	return distance / vectorLength(distance); // Returns vector of length 1
}
