#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <vector>
#include <string>
#include <unordered_map>

class Profile
{
    public:
        using LevelScoresMap = std::unordered_map<int, std::unordered_map<int, int>>; // LevelID, playerID, score

                            Profile();
        void                saveProfile() const;
        bool                loadProfile();
        void                updateData(int levelID, int playerID, int score);
        void                addPlayer(std::string name);
        bool                isLoaded() const;
        void                setCurrentLevel(int);
        int                 getCurrentLevel() const;
        int                 getLevelScore(int levelID, int playerID) const;
        int                 getCumulativeLevelScore(int levelID) const;
        std::vector<int>    getCompletedLevels() const;
        void                clearAllData();

    private:
        LevelScoresMap              mCompletedLevelsInfo; // LevelID, playersScoresMap
        std::vector<std::string>    mPlayerNames;
        bool                        mIsLoaded;
        int                         mCurrentLevel;
};

#endif // PROFILE_HPP
