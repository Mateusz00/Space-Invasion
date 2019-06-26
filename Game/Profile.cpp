#include "Profile.hpp"
#include "Player.hpp"
#include <fstream>

Profile::Profile(std::vector<Player>& players, std::vector<KeyBinding*>& keys)
    : mIsLoaded(false),
      mCurrentLevel(-1),
      mPlayers(players),
      mKeys(keys)
{
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
    clearAllData();
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
            saveFile.get(); // Get rid of '&'

            mCompletedLevelsInfo.insert(std::make_pair(levelID, playersScores));
        }
        saveFile.get(); // Get rid of '#'

        // Load player names
        while(saveFile.peek() != EOF)
        {
            std::string name;
            std::getline(saveFile, name, '%');
            mPlayerNames.push_back(name);
            saveFile >> std::ws;
        }

        mIsLoaded = true;
        saveFile.close();
    }
    updatePlayers();

    return mIsLoaded;
}

void Profile::addPlayer(std::string name)
{
    mPlayerNames.push_back(name);
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

///@return 0 if player didn't complete requested level
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

///@return 0 if player didn't complete requested level
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

int Profile::getOverallPlayerScore(int playerID) const
{
    int score = 0;

    for(const auto& levelScores : mCompletedLevelsInfo)
    {
        auto pos = levelScores.second.find(playerID);

        if(pos != levelScores.second.end())
            score += pos->second;
    }

    return score;
}

std::vector<int> Profile::getCompletedLevels() const
{
    std::vector<int> completedLevels;

    for(const auto& levelScores : mCompletedLevelsInfo)
        completedLevels.emplace_back(levelScores.first);

    return completedLevels;
}

void Profile::clearAllData()
{
    mPlayerNames.clear();
    mCompletedLevelsInfo.clear();
    mIsLoaded = false;
    mPlayers.clear();
    mCurrentLevel = -1;
}

void Profile::updatePlayers()
{
    for(int playerNumber=0; playerNumber < mPlayerNames.size(); ++playerNumber)
        mPlayers.emplace_back(mKeys[playerNumber], playerNumber, mPlayerNames[playerNumber]);
}
