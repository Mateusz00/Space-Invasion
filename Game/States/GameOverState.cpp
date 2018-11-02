#include "GameOverState.hpp"
#include "../Utility.hpp"
#include "../ResourceHolder.hpp"
#include <SFML/Graphics/View.hpp>

GameOverState::GameOverState(Context context, StateStack& stateStack, const std::string& text)
    : State(context, stateStack),
      mWindow(context.window)
{
	mText.setFont(context.fonts.get(Fonts::Sansation));
    mText.setString(text);
	mText.setCharacterSize(60u);
	centerOrigin(mText);

	sf::Vector2f windowSize(context.window.getSize());
	mText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

    mBackground.setFillColor(sf::Color(0, 0, 0, 140));
	mBackground.setSize(mWindow.getView().getSize());
}

bool GameOverState::draw()
{
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mBackground);
	mWindow.draw(mText);
	return false;
}

bool GameOverState::update(sf::Time dt)
{
	mElapsedTime += dt;
	if(mElapsedTime > sf::seconds(4))
	{
		requestStackClear();
		requestStackPush(States::MenuState);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}
