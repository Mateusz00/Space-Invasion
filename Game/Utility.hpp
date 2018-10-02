#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <sstream>
#include <string>

template <typename T>
void centerOrigin(T&);

template <typename T>
std::string toString(const T& value);

int             randomInt(int minNumber, int maxNumberIncluded);
float           vectorLength(sf::Vector2f position);
sf::Vector2f    unitVector(sf::Vector2f distance);
float           toRadian(float degree);
float           toDegree(float rad);


#include "Utility.inl"

#endif // UTILITY_HPP
