#include "GUIContainer.hpp"
#include <SFML/System/Vector2.hpp>

GUIContainer::GUIContainer()
    : mSelected(-1)
{
}

void GUIContainer::push(ComponentPtr component)
{
    mComponents.push_back(std::move(component));

	if (!hasSelection() && mComponents.back()->isSelectable())
    {
        mSelected = mComponents.size() - 1;
        mComponents[mSelected]->select();
    }
}

bool GUIContainer::isSelectable() const
{
	return false;
}

void GUIContainer::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyReleased:
            switch(event.key.code)
            {
            case sf::Keyboard::W:
            case sf::Keyboard::Up:
                selectPrevious();
                break;

            case sf::Keyboard::S:
            case sf::Keyboard::Down:
                selectNext();
                break;

            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                if(hasSelection())
                    mComponents[mSelected]->activate();
                break;
            }
            break;

        case sf::Event::MouseButtonReleased:
        {
            int index;
            if(checkMouseCollision(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), index))
                mComponents[index]->activate();
        }
        break;

        case sf::Event::MouseButtonPressed:
        {
            int index;
            if(checkMouseCollision(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), index))
                mComponents[index]->onMouseClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
        break;
    }

    for(auto& component : mComponents)
    {
        if(component->isActive())
            component->handleEvent(event);
    }
}

sf::FloatRect GUIContainer::getBoundingRect() const
{
	return sf::FloatRect();
}

void GUIContainer::update(sf::Window& window)
{
    int index;

    if(checkMouseCollision(sf::Mouse::getPosition(window), index))
    {
        if(mComponents[index]->isSelectable())
        {
            mComponents[mSelected]->deselect();
            mComponents[index]->select();
            mSelected = index;
        }
    }
}

void GUIContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform(); // relative to container
    for(const auto& component : mComponents)
        target.draw(*component, states);
}

void GUIContainer::selectNext()
{
    mComponents[mSelected]->deselect();

    do
        mSelected = ++mSelected % mComponents.size();
    while(!mComponents[mSelected]->isSelectable());

    mComponents[mSelected]->select();
}

void GUIContainer::selectPrevious()
{
    mComponents[mSelected]->deselect();

    do
        mSelected = (mSelected - 1 + mComponents.size()) % mComponents.size();
    while(!mComponents[mSelected]->isSelectable());

    mComponents[mSelected]->select();
}

bool GUIContainer::hasSelection() const
{
    return mSelected >= 0;
}

sf::FloatRect GUIContainer::getComponentRect(int componentNumber) const
{
    sf::FloatRect rect = mComponents[componentNumber]->getBoundingRect();
    rect = getTransform().transformRect(rect);

    return rect;
}

bool GUIContainer::checkMouseCollision(sf::Vector2i mousePosition, int& index) const
{
    if(!mComponents.empty())
    {
        for(int i=0; i < mComponents.size(); ++i)
        {
            if(getComponentRect(i).contains(static_cast<sf::Vector2f>(mousePosition)))
            {
                index = i;
                return true;
            }
        }
    }

    return false;
}
