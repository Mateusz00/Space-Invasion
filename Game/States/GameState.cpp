#include "GameState.hpp"
#include "../Utility.hpp"
#include "../MusicPlayer.hpp"
#include "../Profile.hpp"
#include <SFML/Graphics/Text.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

GameState::GameState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWorld(context),
      mPlayers(context.players),
      mWindow(context.window),
      mProfile(context.profile)
{
    context.music.playNow(Music::BattleTheme, true);

    for(const Player& player : mPlayers)
        mWorld.addSpaceship(player.getID());
}

bool GameState::draw()
{
    mWorld.draw();
    return false;
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    updatePlayersScore(); // Try to move this inside ifs

    if(!mWorld.hasAlivePlayer())
    {
        requestStackPush(States::MissionFailed);
        getContext().sounds.play(Sound::GameOver);
        getContext().music.pause();
        updateScoresFile();
    }
    else if(mWorld.hasPlayerReachedEnd())
    {
        requestStackPush(States::MissionSuccess);

        std::unordered_map<int, int> playersScores = mWorld.getPlayersScoresMap();
        int levelID = mProfile.getCurrentLevel();
        if(getCurrentCumulativeScore() > mProfile.getCumulativeLevelScore(levelID)) // Saves greater score
        {
            for(Player& player : mPlayers)
                mProfile.updateData(levelID, player.getID(), playersScores.at(player.getID()));
        }
    }

    for(Player& player : mPlayers)
        player.handleRealTimeInput(mWorld.getCommandQueue());

    return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        requestStackPush(States::PauseState);
    else
    {
        for(Player& player : mPlayers)
            player.handleEvent(event, mWorld.getCommandQueue());
    }

    return false;
}

void GameState::updateScoresFile() const
{
    using UserScore = std::pair<int, std::string>;
    std::vector<UserScore> scores(10, UserScore(0, "-"));

    // Read top 10 scores from file
    std::ifstream inputScores("Scores.txt");
    if(inputScores.good())
    {
        for(int i=0; inputScores.peek() != EOF && i < scores.size(); ++i)
            inputScores >> scores[i].first >> scores[i].second;
    }
    inputScores.close();

    // Add players scores to array of previous scores and sort them in descending order
    for(const Player& player : mPlayers)
        scores.push_back(std::make_pair(player.getScore(), player.getName()));

    std::sort(scores.begin(), scores.end(), [](const UserScore& lhs, const UserScore& rhs)
    {
        return lhs.first > rhs.first;
    });

    // Output new top 10 scores to file
    std::ofstream outputScores("Scores.txt", std::ios::out | std::ios::trunc);
    for(int i = 0; i < 9; ++i)
        outputScores << scores[i].first << " " << scores[i].second << " ";

    outputScores.close();
}

void GameState::updatePlayersScore()
{
    std::unordered_map<int, int> playersScores = mWorld.getPlayersScoresMap();

    for(Player& player : mPlayers)
        player.setScore(playersScores[player.getID()]);
}

int GameState::getCurrentCumulativeScore()
{
    std::unordered_map<int, int> playersScores = mWorld.getPlayersScoresMap();
    int score = 0;

    for(const Player& player : mPlayers)
        score += playersScores.at(player.getID());

    return score;
}
