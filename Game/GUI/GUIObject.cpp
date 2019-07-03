#include "GUIObject.hpp"

GUIObject::GUIObject()
    : mIsSelected(false),
      mIsActive(false),
      mHasAbsolutePosition(false)
{
}

GUIObject::~GUIObject()
{
}

void GUIObject::update(sf::Time)
{
}

void GUIObject::select()
{
    mIsSelected = true;
}

void GUIObject::deselect()
{
    mIsSelected = false;
}

bool GUIObject::isSelected()
{
    return mIsSelected;
}

bool GUIObject::isActive() const
{
    return mIsActive;
}

void GUIObject::activate()
{
    mIsActive = true;
}

void GUIObject::deactivate()
{
    mIsActive = false;
}

void GUIObject::handleEvent(const sf::Event&)
{
}

void GUIObject::onMouseClick(sf::Vector2i)
{
    activate();
}

sf::FloatRect GUIObject::getBoundingRect() const
{
}

void GUIObject::setAbsolutePosition()
{
    mHasAbsolutePosition = true;
}

bool GUIObject::hasAbsolutePosition() const
{
    return mHasAbsolutePosition;
}
