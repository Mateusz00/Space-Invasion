#include "GUIObject.hpp"

GUIObject::GUIObject()
    : mIsSelected(false),
      mIsActive(false)
{
}

GUIObject::~GUIObject()
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

sf::FloatRect GUIObject::getBoundingRect() const
{
}
