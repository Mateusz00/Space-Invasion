#ifndef GUI_INPUTBOX_HPP
#define GUI_INPUTBOX_HPP

#include "GUIObject.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <string>

class GUI_InputBox : public GUIObject
{
    public:
                                GUI_InputBox(std::string& output, sf::Vector2f boxSize,
                                             int maxCharacters, bool isForced, const FontHolder&);
        virtual bool            isSelectable() const override;
        virtual void	        activate() override;
        virtual void	        deactivate() override;
        virtual void	        handleEvent(const sf::Event&) override;
        virtual sf::FloatRect   getBoundingRect() const override;
        virtual void	        draw(sf::RenderTarget&, sf::RenderStates) const override;

    private:
        void	                update(sf::Time);

        std::string&            mOutput;
        sf::RectangleShape      mBox;
        sf::RectangleShape      mInputCursor;
        sf::Text                mText;
        std::string             mString;
        int                     mMaxCharacters;
        int                     mInputPosition;
        bool                    mIsForced;
        bool                    mShowCursor;
        sf::Time                mAccumulatedTime;
};

#endif // GUI_INPUTBOX_HPP
