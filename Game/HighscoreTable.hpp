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

class HighscoreTable : public sf::Drawable, public sf::Transformable
{
	public:
						HighscoreTable(sf::RenderWindow&, FontHolder&, int characterSize);
		virtual void 	draw(sf::RenderTarget&, sf::RenderStates) const override;

	private:
		void			loadScores();
		void			positionTexts();
		void			computeBackgroundSize();
		float           getWidestText(std::vector<sf::Text>&) const;

		int 					mCharacterSize;
		sf::RenderWindow&		mWindow;
		FontHolder&				mFonts;
		sf::Text				mHeader;
		sf::RectangleShape		mBackground;
		std::vector<sf::Text> 	mNames;
		std::vector<sf::Text> 	mScores;
};
#endif // HIGHSCORETABLE_HPP
