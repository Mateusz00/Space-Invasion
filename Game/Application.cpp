#include "Application.hpp"
#include "States/StatesID.hpp"
#include "Utility.hpp"
#include "Collision.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
using namespace ApplicationData;

Application::Application()
    : mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D Fighter Jet Game", sf::Style::Close),
      mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mMusicPlayer, mKeyBindings, mPlayers, mSettings, mProfile)),
      mProfile(mPlayers, mKeyBindings)
{
    loadSettings();
    loadResources();
    mWindow.setKeyRepeatEnabled(false);
    mFPSCounter.setFont(mFonts.get(Fonts::Sansation));
    mFPSCounter.setCharacterSize(12u);
    mFPSCounter.setPosition(2.f, 2.f);
    mStateStack.pushState(States::MenuState);
    NarrowPhase::alphaMaskManager.initializeMasks(mTextures);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time  timeSinceUpdate;
    while(mWindow.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        timeSinceUpdate += deltaTime;

        while(timeSinceUpdate >= TIME_PER_UPDATE)
        {
            timeSinceUpdate -= TIME_PER_UPDATE;
            handleEvents();
            update(TIME_PER_UPDATE);

            if(mStateStack.isEmpty())
                mWindow.close();
        }

        updateFPSCounter(deltaTime);
        draw();
    }
}

void Application::draw()
{
    mWindow.clear();
    mStateStack.draw();
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mFPSCounter);
    mWindow.display();
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
    mMusicPlayer.update();
    mSounds.update(dt);
}

void Application::handleEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
        else
            mStateStack.handleEvent(event);
    }
}

void Application::updateFPSCounter(sf::Time dt)
{
    static std::size_t numberOfFrames = 0;
    static sf::Time elapsedTime = sf::Time::Zero;

    numberOfFrames++;
    elapsedTime += dt;
    if(elapsedTime >= sf::seconds(1.f))
    {
        mFPSCounter.setString("FPS: " + toString(numberOfFrames /= elapsedTime.asSeconds()) );
        elapsedTime = sf::Time::Zero;
        numberOfFrames = 0;
    }
}

void Application::loadResources()
{
    mFonts.loadFromFile(Fonts::Sansation,         "Resources/Sansation.ttf");
    mFonts.loadFromFile(Fonts::BPmonoItalics,     "Resources/BPmonoItalics.ttf");
    mFonts.loadFromFile(Fonts::Goodfish,          "Resources/Goodfish.ttf");
    mTextures.loadFromFile(Textures::Background,     "Resources/Background.png");
    mTextures.loadFromFile(Textures::Spaceships,     "Resources/Spaceships.png");
    mTextures.loadFromFile(Textures::Projectiles,    "Resources/Projectiles.png");
    mTextures.loadFromFile(Textures::Pickups,        "Resources/Pickups.png");
    mTextures.loadFromFile(Textures::Particle,       "Resources/Particle.png");
    mTextures.loadFromFile(Textures::Explosion,      "Resources/Explosion.png");
    mTextures.loadFromFile(Textures::TitleScreen,    "Resources/TitleScreen.png");
    mTextures.loadFromFile(Textures::Speaker,        "Resources/Speaker.png");
    mTextures.loadFromFile(Textures::Checkbox,       "Resources/Checkbox.png");
    mTextures.loadFromFile(Textures::FinishLine,     "Resources/FinishLine.png");
    mTextures.loadFromFile(Textures::SettingsButtons,"Resources/Buttons.png");
    mTextures.loadFromFile(Textures::LevelButtons   ,"Resources/LevelButtons.png");
    mTextures.loadFromFile(Textures::Checkmark      ,"Resources/Checkmark.png");
    mTextures.loadFromFile(Textures::Lock           ,"Resources/Lock.png");
    mTextures.loadFromFile(Textures::BossDeath      ,"Resources/BossDeath.png");
}

void Application::loadSettings()
{
    mSettings.load();

    if(mSettings.getMuted())
    {
        mSounds.mute();
        mMusicPlayer.mute();
    }

    mWindow.setFramerateLimit(mSettings.getFramerateLimit());
    mWindow.setVerticalSyncEnabled(mSettings.getVsync());
    mSounds.setVolume(mSettings.getSoundVolume());
    mMusicPlayer.setVolume(mSettings.getMusicVolume());

    mKeyBindings.emplace_back(0, mSettings);
    mKeyBindings.emplace_back(1, mSettings);
}
