#ifndef COLLISIONMAP_HPP
#define COLLISIONMAP_HPP

#include <unordered_map>
#include <functional>

class CollisionMap
{
    public:
        CollisionMap();

    private:
        std::unordered_map<int, std::unordered_map<int, std::function<void()>;
};

#endif // COLLISIONMAP_HPP
