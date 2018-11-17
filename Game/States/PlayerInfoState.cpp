#include "PlayerInfoState.hpp"
#include "../ResourcesID.hpp"
#include "../Player.hpp"
#include "../GUI/GUIButton.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

PlayerInfoState::PlayerInfoState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mPlayers(context.players),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen)),
      mText("Enter Player1 name: ", context.fonts.get(Fonts::Sansation), 20u),
      mInputBox(mPlayerName, sf::Vector2f(400.f, 50.f), 11, true, context.fonts),
      mWindow(context.window),
      mModeConfirmed(false),
      mNumberOfPlayers(1),
      mCurrentPlayer(0)
{
    mPlayers.clear();
    sf::Vector2f windowSize(static_cast<sf::Vector2f>(mWindow.getSize()));

    // Create and position buttons for choosing number of players
    mButtons.setPosition(windowSize * 0.5f);

    std::unique_ptr<GUIButton> onePlayer(new GUIButton(context, GUIButton::Text, "1 Player"));
    onePlayer->setPosition(0.f, windowSize.y * -0.05f);
    onePlayer->setCallback([this]()
    {
        mNumberOfPlayers = 1;
        mModeConfirmed = true;
    });
    mButtons.push(std::move(onePlayer));

    std::unique_ptr<GUIButton> twoPlayers(new GUIButton(context, GUIButton::Text, "2 Players"));
    twoPlayers->setPosition(0.f, windowSize.y * 0.05f);
    twoPlayers->setCallback([this]()
    {
        mNumberOfPlayers = 2;
        mModeConfirmed = true;
    });
    mButtons.push(std::move(twoPlayers));

    //Position text and input box that will be displayed after choosing number of players
    centerOrigin(mText);
    centerOrigin(mInputBox);
    mText.setPosition(windowSize.x * 0.5f, windowSize.y * 0.45f);
    mInputBox.setPosition(windowSize.x * 0.5f, windowSize.y * 0.53f);
}

bool PlayerInfoState::draw()
{
    mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mBackgroundSprite);

	if(!mModeConfirmed)
    {
        mWindow.draw(mButtons);
    }
	else
    {
        mWindow.draw(mText);
        mWindow.draw(mInputBox);
    }

    return false;
}

bool PlayerInfoState::update(sf::Time dt)
{
    if(!mInputBox.isActive() && mModeConfirmed) // After pressing enter input box becomes inActive
    {
        addPlayer(mCurrentPlayer++);

        if(mCurrentPlayer < mNumberOfPlayers)
        {
            mText.setString("Enter Player2 name: ");
            centerOrigin(mText);
            mInputBox.activate();
        }
        else
        {
            requestStackClear();
            requestStackPush(States::GameState);
        }
    }

    if(!mModeConfirmed)
        mButtons.update(mWindow);
    else
        mInputBox.update(dt);

    return false;
}

bool PlayerInfoState::handleEvent(const sf::Event& event)
{
    if(!mModeConfirmed)
        mButtons.handleEvent(event);
    else
        mInputBox.handleEvent(event);

    return false;
}

void PlayerInfoState::addPlayer(int playerNumber)
{
    mPlayers.emplace_back(getContext().keys[playerNumber], playerNumber, mPlayerName); // Change keys variable to array
}
