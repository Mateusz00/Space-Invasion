#ifndef STATE_HPP
#define STATE_HPP

#include "../ResourcesID.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
class StateStack;

class State
{
    public:
        using Ptr = std::unique_ptr<State>;
        struct Context
        {
            Context(sf::RenderWindow*, TextureHolder*, FontHolder*);
            sf::RenderWindow*   window;
            TextureHolder*      textures;
            FontHolder*         fonts;
        };

        State(Context, StateStack*);
        virtual bool draw() = 0;
        virtual bool update(sf::Time) = 0;
        virtual bool handleEvent(const sf::Event&) = 0;

    private:
        Context         mContext;
        StateStack*     mStateStack;
};

#endif // STATE_HPP
