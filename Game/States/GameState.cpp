#include "GameState.hpp"
#include "../Exceptions/XMLParseException.hpp"
#include "../Utility.hpp"
#include "../MusicPlayer.hpp"
#include "../Profile.hpp"
#include "../HighScoresTable.hpp"
#include <SFML/Graphics/Text.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <pugixml.hpp>
#include <unordered_map>

int getLastLevelID();
namespace
{
    int lastLevel = getLastLevelID();
}

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

    if(!mWorld.hasAlivePlayer())
    {
        updatePlayersScore();
        updateScoresFile(mProfile.getCurrentLevel());
        requestStackPush(States::MissionFailed);
        getContext().sounds.play(Sound::GameOver);
        getContext().music.pause();
    }
    else if(mWorld.hasPlayerReachedEnd())
    {
        updatePlayersScore();
        updateScoresFile(mProfile.getCurrentLevel());
        requestStackPush(States::MissionSuccess);

        int levelID = mProfile.getCurrentLevel();
        if(getCurrentCumulativeScore() > mProfile.getCumulativeLevelScore(levelID)) // Saves greater score
        {
            for(Player& player : mPlayers)
                mProfile.updateData(levelID, player.getID(), player.getScore());
        }

        if(levelID == lastLevel)
            updateOverallScore();
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

void GameState::updateOverallScore() const
{
    HighScoresTable scores(getContext().window, getContext().fonts, -1);

    for(Player& player : mPlayers)
    {
        int score = mProfile.getOverallPlayerScore(player.getID());
        scores.addScore(std::make_pair(score, player.getName()));
    }

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
    int score = 0;

    for(const Player& player : mPlayers)
        score += player.getScore();

    return score;
}

int getLastLevelID()
{
    using namespace pugi;

    xml_document doc;
    xml_parse_result result = doc.load_file("Levels/levels.xml");
    if(!result)
        throw XMLParseException(result, "levels.xml");

    xml_node levels = doc.child("levels");
    int id = levels.last_child().attribute("id").as_int(-1);

    if(id == -1)
        throw std::runtime_error("Error: Couldn't find id attribute for final level!");

    return id;
}
