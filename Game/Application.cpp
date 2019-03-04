#include "Application.hpp"
#include "States/StatesID.hpp"
#include "Utility.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

const sf::Time Application::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(1024, 700), "2D Fighter Jet Game", sf::Style::Close),
      mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mMusicPlayer, mKeyBindings, mPlayers, mSettings, mProfile))
{
    loadSettings();
    loadResources();
    mWindow.setKeyRepeatEnabled(false);
    mFPSCounter.setFont(mFonts.get(Fonts::Sansation));
    mFPSCounter.setCharacterSize(12u);
    mFPSCounter.setPosition(2.f, 2.f);
    mStateStack.pushState(States::MenuState);
}

Application::~Application()
{
    for(auto& keyBind : mKeyBindings)
        delete keyBind;
}

void Application::run()
{
    sf::Clock clock;
    sf::Time  timeSinceUpdate;
    while(mWindow.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        timeSinceUpdate += deltaTime;

        while(timeSinceUpdate >= TIME_PER_FRAME)
        {
            timeSinceUpdate -= TIME_PER_FRAME;
            handleEvents();
            update(TIME_PER_FRAME);

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
    mTextures.loadFromFile(Textures::Aircrafts,      "Resources/Airplane1.png");
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

    mKeyBindings.push_back(new KeyBinding(1, mSettings));
    mKeyBindings.push_back(new KeyBinding(2, mSettings));
}

sf::Time Application::getTimePerFrame()
{
    return TIME_PER_FRAME;
}
