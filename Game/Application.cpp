#include "Application.hpp"
#include "States/StatesID.hpp"
#include "Utility.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

Application::Application()
    : TIME_PER_FRAME(sf::seconds(1.f / 60.f)),
      mWindow(sf::VideoMode(1024, 700), "2D Fighter Jet Game", sf::Style::Close),
      mStateStack(State::Context(mWindow, mTextures, mFonts, &mKeyBinding1))
{
    loadResources();
    mWindow.setKeyRepeatEnabled(false);
    mFPSCounter.setFont(mFonts.get(Fonts::Sansation));
    mFPSCounter.setCharacterSize(12u);
    mFPSCounter.setPosition(2.f, 2.f);
    mStateStack.pushState(States::GameState);
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
    mFonts.loadFromFile   (Fonts::Sansation,         "Resources/Sansation.ttf");
    mFonts.loadFromFile   (Fonts::BPmonoItalics,     "Resources/BPmonoItalics.ttf");
    mTextures.loadFromFile(Textures::Background,     "Resources/Background.png");
    mTextures.loadFromFile(Textures::Aircrafts,      "Resources/Airplane1.png");
    mTextures.loadFromFile(Textures::Projectiles,    "Resources/Projectiles.png");
    mTextures.loadFromFile(Textures::Pickups,        "Resources/Pickups.png");
    mTextures.loadFromFile(Textures::Particle,       "Resources/Particle.png");
    mTextures.loadFromFile(Textures::Explosion,      "Resources/Explosion.png");
}
