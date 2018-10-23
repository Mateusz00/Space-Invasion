#include "MenuState.hpp"
#include "../ResourcesID.hpp"
#include "../MusicPlayer.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

MenuState::MenuState(Context context, StateStack& stateStack)
    : State(context, stateStack),
      mWindow(context.window),
      mBackgroundSprite(context.textures.get(Textures::TitleScreen))
{
    context.music.playNow(Music::MenuTheme, true);
    //Add buttons here

    mGUIContainer.setPosition(200.f, 200.f);
}

bool MenuState::draw()
{
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mBackgroundSprite);
    mWindow.draw(mGUIContainer);
    return false;
}

bool MenuState::update(sf::Time dt)
{
    mGUIContainer.update(mWindow);
    return false;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}
