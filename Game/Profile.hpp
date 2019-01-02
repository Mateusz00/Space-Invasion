#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <vector>
#include <string>

class Profile
{
    public:
        Profile();
        ~Profile();
        void saveProfile();
        void loadProfile();

    private:
        std::vector<int>            mCompletedLevels;
        std::vector<int>            mScores;
        std::vector<std::string>    mPlayerNames;
};

#endif // PROFILE_HPP
