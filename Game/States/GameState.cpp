#include "GameState.hpp"
#include "../MusicPlayer.hpp"
#include <fstream>
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
        savePlayersScore();
    }
	else if(mWorld.hasPlayerReachedEnd())
    {
        requestStackPush(States::MissionSuccess);
        savePlayersScore();
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

void GameState::savePlayersScore() const
{
    std::ofstream scoreStream("scores.txt", std::ofstream::app);

    for(const Player& player : mPlayers)
    {
        scoreStream << player.getScore() << "#";
    }
}

void GameState::updatePlayersScore()
{
    std::unordered_map<int, int> playersScores = mWorld.getPlayersScoresMap();

    for(Player& player : mPlayers)
        player.setScore(playersScores[player.getID()]);
}
