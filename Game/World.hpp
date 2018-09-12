#ifndef WORLD_HPP
#define WORLD_HPP

#include "CommandQueue.hpp"
#include "Entities/Aircraft.hpp"
#include "ResourcesID.hpp"
#include "SceneNode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <array>

class World
{
    public:
        World(sf::RenderTarget&, TextureHolder&, FontHolder&);
        CommandQueue& getCommandQueue();
        void update(sf::Time);
        void draw();

    private:
        enum Layer
        {
            Background,
            LowerAir,
            UpperAir,
            LayerCount
        };

        void buildWorld();
        void adaptPlayersVelocity();

        sf::RenderTarget&   mTarget;
        TextureHolder&      mTextures;
        FontHolder&         mFonts;
        SceneNode           mSceneGraph;
        CommandQueue        mCommandQueue;
        Aircraft*           mPlayerAircraft;
        std::array<SceneNode*, LayerCount> mSceneLayers;
};

#endif // WORLD_HPP
