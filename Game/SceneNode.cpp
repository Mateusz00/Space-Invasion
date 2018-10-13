#include "SceneNode.hpp"
#include <functional>
#include <algorithm>

SceneNode::SceneNode(Category::Type category)
    : mCategory(category),
      mParent(nullptr)
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
        currentTransform = node->getTransform() * currentTransform; // We want to apply parent's transform first
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

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::executeCommand(const Command& command, sf::Time dt)
{
    for(const auto& category : command.mCategories)
    {
        if(getCategory() == category)
        {
            command.mAction(*this, dt);
            break;
        }
    }

    for(auto& child : mChildren)
        child->executeCommand(command, dt);
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
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

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    for(auto& child : mChildren)
        child->update(dt, commands);
}

Category::Type SceneNode::getCategory() const
{
    return mCategory;
}

bool SceneNode::isMarkedForRemoval() const
{
    return false;
}

void SceneNode::onRemoval()
{
}

void SceneNode::removeWrecks()
{
    auto wrecksEnd = std::partition(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    std::for_each(mChildren.begin(), wrecksEnd, std::mem_fn(&SceneNode::onRemoval));
    mChildren.erase(mChildren.begin(), wrecksEnd);

    for(auto& child : mChildren)
        child->removeWrecks();
}
