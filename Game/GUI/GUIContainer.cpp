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
    GUIObject* activeButton = checkForToggledButtons();

    if(activeButton != nullptr) // If button is toggled then let him handle events
        activeButton->handleEvent(event);
    else if(event.type == sf::Event::KeyReleased)
    {
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
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        sf::Vector2i tempPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(tempPos);
        int index;

        if(checkMouseCollision(mousePosition, index))
            mComponents[index]->activate();
    }
}

sf::FloatRect GUIContainer::getBoundingRect() const
{
	return sf::FloatRect();
}

void GUIContainer::update(sf::Window& window)
{
    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    int index;

    if(checkMouseCollision(mousePosition, index))
    {
        mComponents[mSelected]->deselect();
        mComponents[index]->select();
        mSelected = index;
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

GUIObject* GUIContainer::checkForToggledButtons() const
{
	for(const auto& component : mComponents)
	{
		if(component->isActive()) // Buttons keep being active if toggle flag is set true
            return component.get();
	}

	return nullptr;
}

sf::FloatRect GUIContainer::getComponentRect(int componentNumber) const
{
    sf::FloatRect rect = mComponents[componentNumber]->getBoundingRect();
    rect = getTransform().transformRect(rect);

    return rect;
}

bool GUIContainer::checkMouseCollision(sf::Vector2f mousePosition, int& index) const
{
    if(!mComponents.empty())
    {
        for(int i = 0; i < mComponents.size(); ++i)
        {
            if(getComponentRect(i).contains(mousePosition) && mComponents[i]->isSelectable())
            {
                index = i;
                return true;
            }
        }
    }

    return false;
}
