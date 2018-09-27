#ifndef WORLD_HPP
#define WORLD_HPP

#include "CommandQueue.hpp"
#include "Entities/Aircraft.hpp"
#include "ResourcesID.hpp"
#include "SceneNode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <array>
#include <list>

class World
{
    public:
                            World(sf::RenderTarget&, TextureHolder&, FontHolder&);
        CommandQueue&       getCommandQueue();
        void                update(sf::Time);
        void                draw();
        sf::FloatRect	    getViewBounds() const;
        sf::FloatRect       getBattlefieldBounds() const;
        void                addCollidable(Entity*);
        void                removeCollidable(Entity*);

    private:
        void buildWorld();
        void adaptPlayersVelocity();
        void initializeSpawnPoints();
        void addSpawnPoint(float x, float y, Aircraft::Type);
        void sortSpawnPoints();
        void spawnEnemies();
        void guideHomingMissiles();
        void adaptPlayersPosition();

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
        SceneNode           mUIGraph;
        CommandQueue        mCommandQueue;
        Aircraft*           mPlayerAircraft;
        sf::View            mView;
        sf::FloatRect       mWorldBounds;
        sf::Vector2f        mPlayerSpawnPosition;
        float               mScrollingSpeed;

        std::vector<Aircraft*>              mActiveEnemies;
        std::vector<SpawnPoint>             mSpawnPoints;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        std::list<Entity*>                  mCollidablesList;
};

#endif // WORLD_HPP
