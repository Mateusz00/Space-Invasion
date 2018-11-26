#ifndef HIGHSCORETABLE_HPP
#define HIGHSCORETABLE_HPP

#include "ResourcesID.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

class HighScoresTable : public sf::Drawable, public sf::Transformable
{
    public:
                        HighScoresTable(sf::RenderWindow&, FontHolder&, int characterSize = 26);
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override;

    private:
        void            loadScores();
        void            positionTexts();

        int                         mCharacterSize;
        sf::RenderWindow&           mWindow;
        FontHolder&                 mFonts;
        sf::RectangleShape          mBackground;
        std::vector<sf::Text>       mNames;
        std::vector<sf::Text>       mScores;
};
#endif // HIGHSCORETABLE_HPP
