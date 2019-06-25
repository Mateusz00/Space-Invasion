#include "GameState.hpp"
#include "../Utility.hpp"
#include "../MusicPlayer.hpp"
#include "../Profile.hpp"
#include "../HighScoresTable.hpp"
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
        updateScoresFile(mProfile.getCurrentLevel());
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

void GameState::updateScoresFile(int levelID) const
{
    HighScoresTable scores(getContext().window, getContext().fonts, levelID);

    for(const Player& player : mPlayers)
        scores.addScore(std::make_pair(player.getScore(), player.getName()));

    scores.saveScores();
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
