#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include "ResourcesID.hpp"

class World
{
    public:
        World(sf::RenderTarget&, TextureHolder&, FontHolder&);
        void update();
        void draw();

    private:
        sf::RenderTarget&   mTarget;
        TextureHolder&      mTextures;
        FontHolder&         mFonts;
        sf::Sprite          mBackground;
};

#endif // WORLD_HPP
