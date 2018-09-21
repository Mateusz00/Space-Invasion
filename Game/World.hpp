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
        CommandQueue&       getCommandQueue();
        void                update(sf::Time);
        void                draw();
        sf::FloatRect	    getViewBounds() const;
        sf::FloatRect       getBattlefieldBounds() const;

    private:
        void buildWorld();
        void adaptPlayersVelocity();
        void initializeSpawnPoints();
        void addSpawnPoint(float x, float y, Aircraft::Type);
        void sortSpawnPoints();
        void spawnEnemies();

        enum Layer
        {
            Background,
            LowerAir,
            UpperAir,
            LayerCount
        };
        struct SpawnPoint
        {
            float x;
            float y;
            Aircraft::Type type;
        };

        sf::RenderTarget&   mTarget;
        TextureHolder&      mTextures;
        FontHolder&         mFonts;
        SceneNode           mSceneGraph;
        CommandQueue        mCommandQueue;
        Aircraft*           mPlayerAircraft;
        sf::View            mView;
        sf::FloatRect       mWorldBounds;
        sf::Vector2f        mPlayerSpawnPosition;
        float               mScrollingSpeed;
        std::vector<SpawnPoint>             mSpawnPoints;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
};

#endif // WORLD_HPP
