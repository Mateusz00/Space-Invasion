#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <vector>
#include <string>

class Profile
{
    public:
                Profile();
                ~Profile();
        void    saveProfile() const;
        bool    loadProfile();
        void    updateData(int levelID, int score);
        bool    isLoaded() const;
        void    setCurrentLevel(int);
        int     getCurrentLevel() const;

    private:
        std::vector<int>            mCompletedLevels;
        std::vector<int>            mScores;
        std::vector<std::string>    mPlayerNames;
        bool                        mIsLoaded;
        int                         mCurrentLevel;
};

#endif // PROFILE_HPP
