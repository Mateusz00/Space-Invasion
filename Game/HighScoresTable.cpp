#include "HighScoresTable.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

/// @param If levelID is 0 then it load's overall scores
HighScoresTable::HighScoresTable(sf::RenderWindow& window, FontHolder& fonts, int characterSize, int levelID)
    : mCharacterSize(characterSize),
      mWindow(window),
      mFonts(fonts),
      mScoresTable(10, PlayerScore(0, "-"))
{
    loadScores(levelID);
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

/// @param If levelID is 0 then it load's overall scores
void HighScoresTable::loadScores(int levelID)
{
    mScoresTable.resize(10);
    std::vector<PlayerScore> scores(10, PlayerScore(0, "-"));

    // Read top 10 scores from file
    std::ifstream inputScores("Scores.txt");
    if(inputScores.good())
    {
        // Load file
        std::string fileContent;
        inputScores.seekg(0, std::ios::end);
        fileContent.resize(inputScores.tellg());
        inputScores.seekg(0, std::ios::beg);
        inputScores.read(&fileContent[0], fileContent.size());

        // Find string that contains scores for requested level
        auto levelScoresBeg = fileContent.find("#" + toString(levelID));
        if(levelScoresBeg != std::string::npos)
        {
            auto levelScoresEnd = fileContent.find('#', levelScoresBeg+1);
            if(levelScoresEnd != std::string::npos)
            {
                std::istringstream levelScores(fileContent.substr(levelScoresBeg+2, levelScoresEnd - (levelScoresBeg+2)));
                for(int i=0; i < mScoresTable.size(); ++i)
                {
                    levelScores >> mScoresTable[i].first >> mScoresTable[i].second;
                }
            }
        }
    }
    inputScores.close();

    // Set texts
    {
        std::ostringstream stream;

        for(int i=0; i < mScoresTable.size(); ++i)
        {
            mNames.emplace_back(mScoresTable[i].second, mFonts.get(Fonts::Sansation), mCharacterSize);

            stream.str("");
            stream << mScoresTable[i].first;
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

void HighScoresTable::addScore(PlayerScore score)
{
    mScoresTable.emplace_back(std::move(score));
}

void HighScoresTable::saveScores()
{
    // TODO
}
