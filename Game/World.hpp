#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include "CommandQueue.hpp"
#include "ResourcesID.hpp"
#include "SceneNode.hpp"

class World
{
    public:
        World(sf::RenderTarget&, TextureHolder&, FontHolder&);
        CommandQueue& getCommandQueue();
        void update();
        void draw();

    private:
        sf::RenderTarget&   mTarget;
        TextureHolder&      mTextures;
        FontHolder&         mFonts;
        sf::Sprite          mBackground;
        SceneNode           mSceneGraph;
        CommandQueue        mCommandQueue;
};

#endif // WORLD_HPP
