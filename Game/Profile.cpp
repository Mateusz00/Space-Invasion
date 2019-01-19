#include "Profile.hpp"
#include <fstream>

Profile::Profile()
    : mIsLoaded(false),
      mCurrentLevel(-1)
{
}

Profile::~Profile()
{
    //dtor
}

void Profile::saveProfile() const
{
    std::ofstream saveFile("save.dat");

    for(const auto& levelInfo : mCompletedLevelsInfo)
    {
        saveFile << levelInfo.first << " "; // levelID

        for(const auto& playersScores : levelInfo.second)
        {
            saveFile << playersScores.first << " "; // playerID
            saveFile << playersScores.second << " "; // playerScore
        }

        saveFile << "&";
    }

    saveFile << "#";

    for(const auto& name : mPlayerNames)
        saveFile << name << "%";

    saveFile.close();
}

bool Profile::loadProfile()
{
    std::ifstream saveFile("save.dat");

    if(saveFile.good())
    {
        // Load data about completed levels
        while(saveFile.peek() != '#')
        {
            int levelID;
            std::unordered_map<int, int> playersScores;

            saveFile >> levelID;
            saveFile >> std::ws;

            // Load all players scores
            while(saveFile.peek() != '&')
            {
                int playerID, score;
                saveFile >> playerID;
                saveFile >> score;
                saveFile >> std::ws;

                playersScores.insert(std::make_pair(playerID, score));
            }

            mCompletedLevelsInfo.insert(std::make_pair(levelID, playersScores));
        }

        // Load player names
        while(saveFile.peek() != EOF)
        {
            std::string name;
            std::getline(saveFile, name, '%');
            mPlayerNames.push_back(name);
        }

        mIsLoaded = true;
        saveFile.close();
    }

    return mIsLoaded;
}

void Profile::updateData(int levelID, int playerID, int score)
{
    (mCompletedLevelsInfo[levelID])[playerID] = score;
}

bool Profile::isLoaded() const
{
    return mIsLoaded;
}

void Profile::setCurrentLevel(int level)
{
    mCurrentLevel = level;
}

int Profile::getCurrentLevel() const
{
    return mCurrentLevel;
}

int Profile::getLevelScore(int levelID, int playerID) const
{
    const auto scoresMap = mCompletedLevelsInfo.find(levelID);

    if(scoresMap != mCompletedLevelsInfo.end())
    {
        const auto playerScoreIter = (scoresMap->second).find(playerID);

        if(playerScoreIter != (scoresMap->second).end())
            return playerScoreIter->second;
    }

    return 0;
}

int Profile::getCumulativeLevelScore(int levelID) const
{
    const auto scoresMap = mCompletedLevelsInfo.find(levelID);
    int score = 0;

    if(scoresMap != mCompletedLevelsInfo.end())
    {
        for(const auto& playerScore : scoresMap->second)
            score += playerScore.second;
    }

    return score;
}
