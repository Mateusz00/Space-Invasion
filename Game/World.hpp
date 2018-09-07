#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include "ResourcesID.hpp"
#include "SceneNode.hpp"

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
        SceneNode           mSceneGraph;
};

#endif // WORLD_HPP
