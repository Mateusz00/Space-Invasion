#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <sstream>
#include <string>

template <typename T>
void centerOrigin(T&);

template <typename T>
std::string toString(const T& value);


#include "Utility.inl"

#endif // UTILITY_HPP
