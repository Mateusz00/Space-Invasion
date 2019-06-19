#ifndef COLLISIONRESPONSEMAP_HPP
#define COLLISIONRESPONSEMAP_HPP

#include "Category.hpp"
#include <unordered_map>
#include <functional>
class Entity;

class CollisionResponseMap
{
    public:
        using Response = std::function<void(Entity&, Entity&)>;
        using ResponseMap = std::unordered_map<Category::Type, Response, std::hash<int>>;

        static void    addResponse(Category::Type, Category::Type, Response);
        static void    useResponse(Entity*, Entity*);

    private:
        static std::unordered_map<Category::Type, ResponseMap, std::hash<int>> mCollisionResponsesMap;
};

template <typename Object, typename Function>
std::function<void(Entity&, Entity&)> castResponse(Function f)
{
    return [=](Entity& entity1, Entity& entity2)
    {
        if(dynamic_cast<Object*>(&entity1) == nullptr)
            throw std::logic_error("Invalid cast!");

        f(static_cast<Object&>(entity1), entity2);
    };
}

template <typename Object1, typename Object2, typename Function>
std::function<void(Entity&, Entity&)> castResponse(Function f)
{
    return [=](Entity& entity1, Entity& entity2)
    {
        if(dynamic_cast<Object1*>(&entity1) == nullptr)
            throw std::logic_error("Invalid cast!");

        if(dynamic_cast<Object2*>(&entity2) == nullptr)
            throw std::logic_error("Invalid cast!");

        f(static_cast<Object1&>(entity1), static_cast<Object2&>(entity2));
    };
}
#endif // COLLISIONRESPONSEMAP_HPP
