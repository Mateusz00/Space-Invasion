#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <vector>
#include <string>
#include <unordered_map>

class Profile
{
    public:
        using LevelScoresMap = std::unordered_map<int, std::unordered_map<int, int>>; // LevelID, playersScoresMap
                Profile();
                ~Profile();
        void    saveProfile() const;
        bool    loadProfile();
        void    updateData(int levelID, int playerID, int score);
        bool    isLoaded() const;
        void    setCurrentLevel(int);
        int     getCurrentLevel() const;
        int     getLevelScore(int levelID, int playerID) const; /// Returns 0 if player didn't complete requested level
        int     getCumulativeLevelScore(int levelID) const; /// Returns 0 if player didn't complete requested level

    private:
        LevelScoresMap              mCompletedLevelsInfo; // LevelID, playersScoresMap
        std::vector<std::string>    mPlayerNames;
        bool                        mIsLoaded;
        int                         mCurrentLevel;
};

#endif // PROFILE_HPP
