#ifndef GUILABEL_HPP
#define GUILABEL_HPP

#include "GUIObject.hpp"
#include "../ResourcesID.hpp"
#include <SFML/Graphics/Text.hpp>

class GUILabel : public GUIObject
{
    public:
                                GUILabel(const std::string&, const FontHolder&);
        void			        setText(const std::string&);
        virtual bool	        isSelectable() const override;
        virtual void	        handleEvent(const sf::Event&) override;
        virtual sf::FloatRect   getBoundingRect() const override;

    private:
        void		            draw(sf::RenderTarget&, sf::RenderStates) const;

        sf::Text    mText;
};

#endif // GUILABEL_HPP
