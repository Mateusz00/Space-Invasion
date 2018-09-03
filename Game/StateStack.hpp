#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "ResourcesID.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/NonCopyable.hpp>
class State;

class StateStack : private sf::NonCopyable
{
    public:
        struct Context
        {
            Context(sf::RenderWindow*, TextureHolder*, FontHolder*);
            sf::RenderWindow*   window;
            TextureHolder*      textures;
            FontHolder*         fonts;
        };
        explicit StateStack(Context);

    private:
        Context mContext;
};

#endif // STATESTACK_HPP
