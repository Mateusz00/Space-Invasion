#ifndef AABB_HPP
#define AABB_HPP

#include <SFML/Graphics/Rect.hpp>

struct AABB
{
	sf::FloatRect aabb;
	int entityID;
};

template <typename Rect>
Rect mergeRects(const Rect& first, const Rect& second) const
{
    return Rect(std::min(first.left, second.left),
                std::min(first.top, second.top),
                std::max(first.left + first.width, second.left + second.width),
                std::max(first.top  + first.height, second.top  + second.height));
}

template <typename Rect>
float getSurfaceArea(const Rect& rect) const
{
    return ((rect.left + rect.width) * (rect.top + rect.height));
}

#endif // AABB_HPP
