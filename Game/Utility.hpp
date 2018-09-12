#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <sstream>
#include <string>


void centerOrigin(sf::Sprite&);
void centerOrigin(sf::Text&);
template <typename T>
std::string toString(const T& value);


#include "toString.inl"

#endif // UTILITY_HPP
