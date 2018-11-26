#include "HighScoresTable.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <sstream>
#include <fstream>

HighScoresTable::HighScoresTable(sf::RenderWindow& window, FontHolder& fonts, int characterSize)
    : mCharacterSize(characterSize),
      mWindow(window),
      mFonts(fonts)
{
    loadScores();
    positionTexts();

    mBackground.setSize((sf::Vector2f(characterSize * 20, characterSize * 16)));
    mBackground.setFillColor(sf::Color(7, 7, 7, 160));
    mBackground.setOutlineColor(sf::Color(120, 50, 60, 160));
    mBackground.setOutlineThickness(3.f);
}

void HighScoresTable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    mWindow.setView(mWindow.getDefaultView());
    states.transform *= getTransform();

    mWindow.draw(mBackground, states);

    for(const auto& drawable : mNames)
        mWindow.draw(drawable, states);

    for(const auto& drawable : mScores)
        mWindow.draw(drawable, states);
}

void HighScoresTable::loadScores()
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

            stream.str("");
            stream << scores[i].first;
            mScores.emplace_back(stream.str(), mFonts.get(Fonts::Sansation), mCharacterSize);
        }
    }
}

void HighScoresTable::positionTexts()
{
    for(int i=0; i < mNames.size(); ++i)
        mNames[i].setPosition(10.f, mCharacterSize * 1.5f * i + 10.f);

    for(int i=0; i < mScores.size(); ++i)
        mScores[i].setPosition(mCharacterSize * 13.f, mCharacterSize * 1.5f * i + 10.f);
}
