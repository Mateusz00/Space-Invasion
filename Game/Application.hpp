#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
    public:
        Application();
        void run();

    private:
        void draw();
        void update(sf::Time dt);
        void handleEvents();
        void updateFPSCounter(sf::Time dt);

        const sf::Time       TIME_PER_FRAME;
        sf::RenderWindow     mWindow;
        sf::Font             mFont;
        sf::Text             mFPSCounter;
};

#endif // APPLICATION_HPP
