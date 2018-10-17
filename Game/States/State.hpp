#ifndef STATE_HPP
#define STATE_HPP

#include "../ResourcesID.hpp"
#include "../KeyBinding.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
class StateStack;
class SoundPlayer;

class State
{
    public:
        using Ptr = std::unique_ptr<State>;
        struct Context
        {
            Context(sf::RenderWindow&, TextureHolder&, FontHolder&, SoundPlayer&, KeyBinding*);
            sf::RenderWindow&   window;
            TextureHolder&      textures;
            FontHolder&         fonts;
            SoundPlayer&        sounds;
            KeyBinding*         keys1;
        };

                        State(Context, StateStack&);
        virtual bool    draw() = 0;
        virtual bool    update(sf::Time) = 0;
        virtual bool    handleEvent(const sf::Event&) = 0;

    private:
        Context         mContext;
        StateStack&     mStateStack;
};

#endif // STATE_HPP
