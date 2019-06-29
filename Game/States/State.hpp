#ifndef STATE_HPP
#define STATE_HPP

#include "../ResourcesID.hpp"
#include "StatesID.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
class StateStack;
class SoundPlayer;
class MusicPlayer;
class Player;
class Profile;
class Settings;
class KeyBinding;

class State
{
    public:
        using Ptr = std::unique_ptr<State>;
        struct Context
        {
            Context(sf::RenderWindow&, TextureHolder&, FontHolder&, SoundPlayer&, MusicPlayer&,
                    std::vector<KeyBinding>&, std::vector<Player>&, Settings&, Profile&);

            sf::RenderWindow&           window;
            TextureHolder&              textures;
            FontHolder&                 fonts;
            SoundPlayer&                sounds;
            MusicPlayer&                music;
            std::vector<KeyBinding>&    keys;
            std::vector<Player>&        players;
            Settings&                   settings;
            Profile&                    profile;
        };

                        State(Context, StateStack&);
        virtual         ~State();
        virtual bool    draw() = 0;
        virtual bool    update(sf::Time) = 0;
        virtual bool    handleEvent(const sf::Event&) = 0;
        void            requestStackPush(States::ID);
        void            requestStackPop();
        void            requestStackClear();
        State::Context  getContext() const;

    private:
        Context         mContext;
        StateStack&     mStateStack;
};

#endif // STATE_HPP
