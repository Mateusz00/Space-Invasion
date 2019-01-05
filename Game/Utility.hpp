#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

struct StringKeyMap
{
    StringKeyMap();
    std::unordered_map<std::string, sf::Keyboard::Key> stringToKey;
};

template <typename T>
void centerOrigin(T&);

template <typename Obj, typename Txt>
void centerText(const Obj&, Txt&);

template <typename T>
std::string toString(const T& value);

std::string         toString(sf::Keyboard::Key);
sf::Keyboard::Key   toKey(const std::string&);
int                 randomInt(int minNumber, int maxNumberIncluded);
float               vectorLength(sf::Vector2f position);
sf::Vector2f        unitVector(sf::Vector2f distance);
float               toRadian(float degree);
float               toDegree(float rad);


#include "Utility.inl"

#endif // UTILITY_HPP
