#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "StateStack.hpp"
#include "ResourcesID.hpp"
#include "SoundPlayer.hpp"
#include "MusicPlayer.hpp"
#include "Player.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
    public:
             Application();
             ~Application();
        void run();

    private:
        void draw();
        void update(sf::Time dt);
        void handleEvents();
        void updateFPSCounter(sf::Time dt);
        void loadResources();

        const sf::Time              TIME_PER_FRAME;
        sf::RenderWindow            mWindow;
        FontHolder                  mFonts;
        TextureHolder               mTextures;
        SoundPlayer                 mSounds;
        MusicPlayer                 mMusicPlayer;
        std::vector<KeyBinding*>    mKeyBindings;
        sf::Text                    mFPSCounter;
        StateStack                  mStateStack;
        std::vector<Player>         mPlayers;
};

#endif // APPLICATION_HPP
