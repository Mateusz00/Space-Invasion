#include "HighScoresTable.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <sstream>
#include <string>
#include <fstream>

/// @param If levelID is -1 then it load's overall scores
HighScoresTable::HighScoresTable(sf::RenderWindow& window, FontHolder& fonts, int levelID, int characterSize)
    : mCharacterSize(characterSize),
      mCurrentLevelID(levelID),
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

void HighScoresTable::loadFile(std::string& str, std::string fileName)
{
    std::ifstream file(fileName);

    if(file.good())
    {
        file.seekg(0, std::ios::end);
        str.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&str[0], str.size());
    }

    file.close();
}

/// @param If levelID is -1 then it load's overall scores
void HighScoresTable::loadScores(int levelID)
{
    mCurrentLevelID = levelID;
    mScoresTable.resize(10);

    // Read top 10 scores from file
    std::ifstream inputScores("Scores.txt");
    if(inputScores.good())
    {
        // Load file
        std::string fileContent;
        loadFile(fileContent, "Scores.txt");

        // Find string that contains scores for requested level
        std::string levelTag = "#" + toString(levelID);
        auto levelScoresBeg = fileContent.find(levelTag);

        if(levelScoresBeg != std::string::npos)
        {
            auto levelScoresEnd = fileContent.find('#', levelScoresBeg+1);
            if(levelScoresEnd != std::string::npos)
            {
                auto scoresBeg = levelScoresBeg + levelTag.size();
                std::istringstream levelScores(fileContent.substr(scoresBeg, levelScoresEnd - scoresBeg));

                for(int i=0; i < mScoresTable.size(); ++i)
                    levelScores >> mScoresTable[i].first >> mScoresTable[i].second;
            }
        }
        else
        {
            for(auto& playerScore : mScoresTable)
            {
                playerScore.first = 0;
                playerScore.second = "-";
            }
        }
    }
    inputScores.close();

    // Set texts
    for(int i=0; i < mScoresTable.size(); ++i)
    {
        mNames.emplace_back(mScoresTable[i].second, mFonts.get(Fonts::Sansation), mCharacterSize);
        mScores.emplace_back(toString(mScoresTable[i].first), mFonts.get(Fonts::Sansation), mCharacterSize);
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
    std::sort(mScoresTable.begin(), mScoresTable.end(), [](const PlayerScore& lhs, const PlayerScore& rhs)
    {
        return lhs.first > rhs.first;
    });

    // Create new string containing updated scores
    std::ostringstream newScores;
    newScores << "#" << toString(mCurrentLevelID);

    for(int i = 0; i < 9; ++i)
        newScores << mScoresTable[i].first << " " << mScoresTable[i].second << " ";

    newScores << mScoresTable[9].first << " " << mScoresTable[9].second << "#";

    // Load file contents
    std::string newFileContents;
    loadFile(newFileContents, "Scores.txt");

    // Find string that contains scores for requested level and replace it
    auto levelScoresBeg = newFileContents.find("#" + toString(mCurrentLevelID));
    bool isCorrupted = false;

    if(levelScoresBeg != std::string::npos)
    {
        auto levelScoresEnd = newFileContents.find('#', levelScoresBeg+1);

        if(levelScoresEnd != std::string::npos)
            newFileContents.replace(levelScoresBeg, (levelScoresEnd - levelScoresBeg)+1, newScores.str());
        else
            isCorrupted = true;
    }
    else
    {
        newFileContents.append(newScores.str());
    }

    // If there is something wrong with data in file, then save only correct string to file
    if(isCorrupted)
        newFileContents = newScores.str();

    std::ofstream outputScores("Scores.txt", std::ios::out | std::ios::trunc);
    outputScores << newFileContents;
    outputScores.close();
}
