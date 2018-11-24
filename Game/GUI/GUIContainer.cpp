#include "GUIContainer.hpp"
#include <SFML/System/Vector2.hpp>

GUIContainer::GUIContainer(bool allowKeyboardNavigation, bool mVerticalNavigation)
    : mSelected(-1),
      mAllowKeyboardNavigation(allowKeyboardNavigation),
      mVerticalNavigation(mVerticalNavigation)
{
}

void GUIContainer::push(ComponentPtr component)
{
    mComponents.push_back(std::move(component));
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
                    if(mAllowKeyboardNavigation && mVerticalNavigation)
                        selectPrevious();
                    break;

                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    if(mAllowKeyboardNavigation && mVerticalNavigation)
                        selectNext();
                    break;

                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    if(mAllowKeyboardNavigation && !mVerticalNavigation)
                        selectPrevious();
                    break;

                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    if(mAllowKeyboardNavigation && !mVerticalNavigation)
                        selectNext();
                    break;

                case sf::Keyboard::Enter:
                case sf::Keyboard::Space:
                    if(hasSelection() && mAllowKeyboardNavigation)
                        mComponents[mSelected]->activate();
                    break;
            }
            break;

        case sf::Event::MouseButtonPressed:
        {
            int index;
            if(checkMouseCollision(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), index))
            {
                if(index != mSelected && mComponents[index]->isSelectable())
                {
                    restart();
                    mComponents[index]->select();
                    mSelected = index;
                }

                mComponents[index]->onMouseClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
        }
        break;

        case sf::Event::MouseMoved:
        {
            int index;
            if(checkMouseCollision(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), index))
            {
                if(index != mSelected && mComponents[index]->isSelectable())
                {
                    restart();
                    mComponents[index]->select();
                    mSelected = index;
                }
                else if(!mComponents[index]->isSelectable())
                    restart();
            }
            else // If didn't collide with anything after moving mouse then get rid of selection and deactivate object
                restart();
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

void GUIContainer::setVerticalNavigation(bool flag)
{
    mVerticalNavigation = flag;
}

void GUIContainer::useKeyboardNavigation(bool flag)
{
    mAllowKeyboardNavigation = flag;
}

void GUIContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(const auto& component : mComponents)
    {
        if(component->hasAbsolutePosition())
            target.draw(*component, states);
    }

    // Draw relative to container
    states.transform *= getTransform();

    for(const auto& component : mComponents)
    {
        if(!component->hasAbsolutePosition())
            target.draw(*component, states);
    }
}

void GUIContainer::selectNext()
{
    deselect();

    do
        mSelected = ++mSelected % mComponents.size();
    while(!mComponents[mSelected]->isSelectable());

    mComponents[mSelected]->select();
}

void GUIContainer::selectPrevious()
{
    deselect();

    do
        mSelected = (mSelected - 1 + mComponents.size()) % mComponents.size();
    while(!mComponents[mSelected]->isSelectable());

    mComponents[mSelected]->select();
}

void GUIContainer::deselect()
{
    if(hasSelection())
        mComponents[mSelected]->deselect();
}

bool GUIContainer::hasSelection() const
{
    return mSelected >= 0;
}

sf::FloatRect GUIContainer::getComponentRect(int componentNumber) const
{
    sf::FloatRect rect = mComponents[componentNumber]->getBoundingRect();

    if(!mComponents[componentNumber]->hasAbsolutePosition())
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

void GUIContainer::restart()
{
    if(hasSelection())
    {
        mComponents[mSelected]->deactivate();
        deselect();
        mSelected = -1;
    }
}
