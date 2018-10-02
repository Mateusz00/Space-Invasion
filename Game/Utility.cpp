#include "Utility.hpp"
#include <cmath>
#include <ctime>
#include <random>

namespace
{
    auto seed = static_cast<unsigned int>(std::time(nullptr));
	auto RandomEngine = std::default_random_engine(seed);
}

int randomInt(int minNumber, int maxNumberIncluded)
{
	std::uniform_int_distribution<int> distribution(minNumber, maxNumberIncluded);
	return distribution(RandomEngine);
}

float vectorLength(sf::Vector2f dist)
{
    return std::sqrt(dist.x * dist.x + dist.y * dist.y);
}

sf::Vector2f unitVector(sf::Vector2f distance)
{
    if(distance == sf::Vector2f(0.f, 0.f))
        throw std::logic_error("unitVector: Wrong length of vector!");

	return distance / vectorLength(distance); // Returns vector of length 1
}

float toRadian(float degree)
{
    return 3.1415f / 180.f * degree;
}

float toDegree(float rad)
{
    return 180.f / 3.1415f * rad;
}
