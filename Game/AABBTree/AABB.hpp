#ifndef AABB_HPP
#define AABB_HPP

#include <SFML/Graphics/Rect.hpp>

struct AABB
{
	sf::FloatRect rect;
	int entityID = -1;
};

template <typename Rect>
Rect mergeRects(const Rect& first, const Rect& second)
{
    auto left    = std::min(first.left, second.left);
    auto top     = std::min(first.top, second.top);
    auto width   = std::max(first.left + first.width,  second.left + second.width) - left;
    auto height  = std::max(first.top + first.height, second.top + second.height) - top;

    return Rect(left, top, width, height);
}

template <typename Rect>
float getSurfaceArea(const Rect& rect)
{
    return (rect.width * rect.height);
}

#endif // AABB_HPP
