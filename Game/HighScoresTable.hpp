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
        using PlayerScore = std::pair<int, std::string>;

                        HighScoresTable(sf::RenderWindow&, FontHolder&, int levelID = 0, int characterSize = 26);
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override;
        void            addScore(PlayerScore);
        void            saveScores();
        void            loadScores(int levelID = 0);

    private:
        void            loadFile(std::string& str, std::string fileName);
        void            positionTexts();

        int                         mCharacterSize;
        int                         mCurrentLevelID;
        sf::RenderWindow&           mWindow;
        FontHolder&                 mFonts;
        sf::RectangleShape          mBackground;
        std::vector<sf::Text>       mNames;
        std::vector<sf::Text>       mScores;
        std::vector<PlayerScore>    mScoresTable;
};
#endif // HIGHSCORETABLE_HPP
