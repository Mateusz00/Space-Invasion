#include "HighscoreTable.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <sstream>
#include <fstream>

HighscoreTable::HighscoreTable(sf::RenderWindow& window, FontHolder& fonts, int characterSize)
	: mCharacterSize(characterSize),
	  mWindow(window),
	  mFonts(fonts),
	  mHeader("Highscores", mFonts.get(Fonts::Sansation), characterSize * 2)
{
	loadScores();
	positionTexts();
	computeBackgroundSize();
	mBackground.setFillColor(sf::Color(17, 17, 17));
	mBackground.setOutlineColor(sf::Color::Black);
	mBackground.setOutlineThickness(3.f);
}

void HighscoreTable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	mWindow.setView(mWindow.getDefaultView());
	states.transform *= getTransform();

    mWindow.draw(mBackground, states);

	for(const auto& drawable : mNames)
		mWindow.draw(drawable, states);

	for(const auto& drawable : mScores)
		mWindow.draw(drawable, states);
}

void HighscoreTable::loadScores()
{
	using UserScore = std::pair<int, std::string>;
    std::vector<UserScore> scores(10, UserScore(0, "-"));

    // Read top 10 scores from file
    std::ifstream inputScores("Scores.txt");
    if(inputScores.good())
    {
        for(int i=0; inputScores.peek() != EOF && i < scores.size(); ++i)
            inputScores >> scores[i].first >> scores[i].second;
    }
	inputScores.close();

	// Set texts
	{
		std::ostringstream stream;

		for(int i=0; i < scores.size(); ++i)
		{
			mNames.emplace_back(scores[i].second, mFonts.get(Fonts::Sansation), mCharacterSize);

			stream.clear();
			stream << scores[i].first;
			mScores.emplace_back(stream.str(), mFonts.get(Fonts::Sansation), mCharacterSize);
		}
	}
}

void HighscoreTable::positionTexts()
{
    float xOffset = getWidestText(mScores);

	for(int i=0; i < mNames.size(); ++i)
		mNames[i].setPosition(0.f, mCharacterSize * 1.5f);

	for(int i=0; i < mScores.size(); ++i)
		mScores[i].setPosition(xOffset + 50.f, mCharacterSize * 1.5f);
}

void HighscoreTable::computeBackgroundSize()
{
	sf::Vector2f topLeft(mNames[0].getGlobalBounds().left, mNames[0].getGlobalBounds().top);
	float topRight = mScores[0].getGlobalBounds().left + getWidestText(mScores);
	float bottomLeft = mNames[mNames.size()-1].getGlobalBounds().top + mNames[mNames.size()-1].getGlobalBounds().height;
	mBackground.setSize(sf::Vector2f(topRight - topLeft.x, bottomLeft - topLeft.y));
}

float HighscoreTable::getWidestText(std::vector<sf::Text>& texts) const
{
	float xOffset = 0.f;

	for(int i=0; i < texts.size(); ++i)
	{
		if(float boxWidth = texts[i].getLocalBounds().width > xOffset)
			xOffset = boxWidth;
	}

	return xOffset;
}
