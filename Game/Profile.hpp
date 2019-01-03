#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <vector>
#include <string>

class Profile
{
    public:
        Profile();
        ~Profile();
        void saveProfile() const;
        void loadProfile();
        void updateData(int levelID, int score);
        bool isLoaded() const;

    private:
        std::vector<int>            mCompletedLevels;
        std::vector<int>            mScores;
        std::vector<std::string>    mPlayerNames;
        bool                        mIsLoaded;
};

#endif // PROFILE_HPP
