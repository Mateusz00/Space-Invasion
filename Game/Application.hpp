#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ApplicationData.hpp"
#include "StateStack.hpp"
#include "ResourcesID.hpp"
#include "SoundPlayer.hpp"
#include "MusicPlayer.hpp"
#include "Settings.hpp"
#include "Player.hpp"
#include "Profile.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
    public:
                            Application();
        void                run();
    private:
        void draw();
        void update(sf::Time dt);
        void handleEvents();
        void updateFPSCounter(sf::Time dt);
        void loadResources();
        void loadSettings();

        sf::RenderWindow            mWindow;
        FontHolder                  mFonts;
        TextureHolder               mTextures;
        SoundPlayer                 mSounds;
        MusicPlayer                 mMusicPlayer;
        std::vector<KeyBinding>     mKeyBindings;
        sf::Text                    mFPSCounter;
        StateStack                  mStateStack;
        std::vector<Player>         mPlayers;
        Settings                    mSettings;
        Profile                     mProfile;
};

#endif // APPLICATION_HPP
