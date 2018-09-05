#include "SceneNode.hpp"
#include <functional>

SceneNode::SceneNode()
    : mParent(nullptr)
{
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform currentTransform;
    for(const SceneNode* node = this; node != nullptr; node = node->mParent)
    {
        currentTransform *= node->getTransform();
    }
    return currentTransform;
}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

void SceneNode::eraseChild(Ptr child)
{
    auto foundChild = std::find_if(mChildren.begin(), mChildren.end(),
                                   [&](Ptr& x){return x == child;});
    mChildren.erase(foundChild);
}

void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto& child : mChildren)
        child->draw(target, states);
}

void SceneNode::updateCurrent(sf::Time dt)
{
}

void SceneNode::updateChildren(sf::Time dt)
{
    for(auto& child : mChildren)
        child->update(dt);
}
