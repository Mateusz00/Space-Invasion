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
        CommandQueue&   getCommandQueue();
        void            update(sf::Time);
        void            draw();

    private:
        void buildWorld();
        void adaptPlayersVelocity();

        enum Layer
        {
            Background,
            LowerAir,
            UpperAir,
            LayerCount
        };

        sf::RenderTarget&   mTarget;
        TextureHolder&      mTextures;
        FontHolder&         mFonts;
        SceneNode           mSceneGraph;
        CommandQueue        mCommandQueue;
        Aircraft*           mPlayerAircraft;
        sf::View            mView;
        std::array<SceneNode*, LayerCount> mSceneLayers;
};

#endif // WORLD_HPP
