#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Category.hpp"
#include <SFML/Window/Event.hpp>
#include <functional>
#include <vector>
#include <exception>
class SceneNode;

struct Command
{
    std::function<void(SceneNode&, sf::Time)>   mAction;
    Category::Type                              mCategories;
};

template <typename Object, typename Function>
std::function<void(SceneNode&, sf::Time)> castFunctor(Function f)
{
    return [=](SceneNode& node, sf::Time dt)
    {
        if(dynamic_cast<Object*>(&node) == nullptr)
            throw std::logic_error("Invalid cast!");

        f(static_cast<Object&>(node), dt);
    };
}
#endif // COMMAND_HPP
