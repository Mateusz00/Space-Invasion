#include "GUI_InputBox.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/Color.hpp>

GUI_InputBox::GUI_InputBox(std::string& output, sf::Vector2f boxSize, int maxCharacters, bool isForced, const FontHolder& fonts)
    : mOutput(output),
      mBox(boxSize),
      mInputCursor(sf::Vector2f(1.f, boxSize.y - 4.f)),
      mMaxCharacters(maxCharacters),
      mInputPosition(0),
      mIsForced(isForced),
      mShowCursor(false)
{
    if(mIsForced)
        activate();

    mBox.setFillColor(sf::Color(61, 189, 204, 180));

    mInputCursor.setFillColor(sf::Color::Black);
    mInputCursor.setPosition(2.f, 2.f);

    mText.setFont(fonts.get(Fonts::Sansation));
    mText.setCharacterSize(static_cast<unsigned int>(boxSize.y - 4.f));
    mText.setPosition(2.f, 2.f);
}

bool GUI_InputBox::isSelectable() const
{
    return false;
}

void GUI_InputBox::activate()
{
    GUIObject::activate();
    mShowCursor = true;
}

void GUI_InputBox::deactivate()
{
    GUIObject::deactivate();
    mShowCursor = false;
}

void GUI_InputBox::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyReleased:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Left:
                    if(mInputPosition > 0)
                        --mInputPosition;
                    return;

                case sf::Keyboard::Right:
                    if(mInputPosition < mString.size())
                        ++mInputPosition;
                    return;

                case sf::Keyboard::BackSpace:
                    if(mInputPosition > 0)
                    {
                        mString.erase(--mInputPosition, 1);
                        mText.setString(mString);
                    }
                    return;

                case sf::Keyboard::Enter:
                    mOutput.assign(mString);
                    deactivate();
                    return;
            }
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            sf::Vector2i tempPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f mousePosition = static_cast<sf::Vector2f>(tempPos);

            if(!getBoundingRect().contains(mousePosition))
                deactivate();
            break;
        }

        case sf::Event::TextEntered:
        {
            if(event.text.unicode < 128)
            {
                char character = static_cast<char>(event.text.unicode);
                mString.insert(mInputPosition, &character);
                ++mInputPosition;
                mText.setString(mString);
            }
            break;
        }
    }
}

sf::FloatRect GUI_InputBox::getBoundingRect() const
{
    return getTransform().transformRect(mBox.getGlobalBounds());
}

void GUI_InputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBox, states);
    target.draw(mText, states);
    if(isActive() && mShowCursor)
        target.draw(mInputCursor, states);
}

void GUI_InputBox::update(sf::Time dt)
{
    // Make cursor blink
    if(isActive())
    {
        mAccumulatedTime += dt;
        if(mAccumulatedTime >= sf::seconds(0.3f))
        {
            mAccumulatedTime = sf::Time::Zero;
            mShowCursor = !mShowCursor;
        }
    }
}
// TODO: Add function updating position of input cursor and mIsForced checks
