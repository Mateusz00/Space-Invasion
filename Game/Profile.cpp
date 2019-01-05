#include "Profile.hpp"
#include <fstream>

Profile::Profile()
    : mIsLoaded(false)
{
}

Profile::~Profile()
{
    //dtor
}

void Profile::saveProfile() const
{
    std::ofstream saveFile("save.dat");

    for(const auto& levelID : mCompletedLevels)
        saveFile << levelID << " ";

    saveFile << "#";

    for(const auto& score : mScores)
        saveFile << score << " ";

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
            saveFile >> levelID;
            saveFile >> std::ws;
            mCompletedLevels.push_back(levelID);
        }

        // Load scores
        while(saveFile.peek() != '#')
        {
            int score;
            saveFile >> score;
            saveFile >> std::ws;
            mScores.push_back(score);
        }

        //Load player names
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

void Profile::updateData(int levelID, int score)
{
    mCompletedLevels.push_back(levelID);
    mScores.push_back(score);
}

bool Profile::isLoaded() const
{
    return mIsLoaded;
}
