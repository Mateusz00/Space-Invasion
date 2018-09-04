#include "Application.hpp"
#include "Utility/toString.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

Application::Application()
    : TIME_PER_FRAME(sf::seconds(1.f / 60.f)),
      mWindow(sf::VideoMode(1024, 768), "2D Fighter Jet Game", sf::Style::Close),
      mStateStack(State::Context(&mWindow, &mTextures, &mFonts))
{
    mWindow.setKeyRepeatEnabled(false);
    mFonts.loadFromFile(Fonts::Sansation, "Resources/Sansation.ttf");
    mFPSCounter.setFont(mFonts.get(Fonts::Sansation));
    mFPSCounter.setCharacterSize(12u);
    mFPSCounter.setPosition(2.f, 2.f);
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
    static std::size_t  numberOfFrames = 0;
    static sf::Time     elapsedTime = sf::Time::Zero;

    numberOfFrames++;
    elapsedTime += dt;
    if(elapsedTime >= sf::seconds(1.f))
    {
        mFPSCounter.setString("FPS: " + toString(numberOfFrames /= elapsedTime.asSeconds()) );
        elapsedTime = sf::Time::Zero;
        numberOfFrames = 0;
    }
}
