#ifndef STATE_HPP
#define STATE_HPP

#include "../ResourcesID.hpp"
#include "../KeyBinding.hpp"
#include "StatesID.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
class StateStack;
class SoundPlayer;
class MusicPlayer;

class State
{
    public:
        using Ptr = std::unique_ptr<State>;
        struct Context
        {
            Context(sf::RenderWindow&, TextureHolder&, FontHolder&, SoundPlayer&, MusicPlayer&, KeyBinding*);
            sf::RenderWindow&   window;
            TextureHolder&      textures;
            FontHolder&         fonts;
            SoundPlayer&        sounds;
            MusicPlayer&        music;
            KeyBinding*         keys1;
        };

                        State(Context, StateStack&);
        virtual         ~State();
        virtual bool    draw() = 0;
        virtual bool    update(sf::Time) = 0;
        virtual bool    handleEvent(const sf::Event&) = 0;
        void            requestStackPush(States::ID);
        void            requestStackPop();
        void            requestStateClear();

    private:
        Context         mContext;
        StateStack&     mStateStack;
};

#endif // STATE_HPP
