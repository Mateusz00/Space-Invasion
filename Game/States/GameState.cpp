#include "GameState.hpp"
#include "../MusicPlayer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

GameState::GameState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWorld(context)
{
    mPlayers.emplace_back(context.keys1, 0);
    context.music.playNow(Music::BattleTheme, true);
}

bool GameState::draw()
{
    mWorld.draw();
    return false;
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    updatePlayersScore();

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
        updateScoresFile();
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
    std::vector<int> scores(10);

    // Read top 10 scores from file
    std::ifstream inputScores("Scores.txt");
    if(inputScores.good())
    {
        std::string text;
        std::getline(inputScores, text);
        std::istringstream stream(text);

        for(int i=0; stream.peek() != EOF; ++i)
            stream >> scores[i];
    }
    inputScores.close();

    // Add players scores to array of previous scores and sort them in descending order
    for(const Player& player : mPlayers)
        scores.push_back(player.getScore());
    std::sort(scores.begin(), scores.end(), [](const int& lhs, const int& rhs){return lhs > rhs;});

    // Output new top 10 scores to file
    std::ofstream outputScores("Scores.txt", std::ios::out | std::ios::trunc);
    for(int i = 0; i < 10; ++i)
        outputScores << scores[i] << " ";
}

void GameState::updatePlayersScore()
{
    std::unordered_map<int, int> playersScores = mWorld.getPlayersScoresMap();

    for(Player& player : mPlayers)
        player.setScore(playersScores[player.getID()]);
}
