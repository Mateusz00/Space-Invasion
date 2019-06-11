#ifndef WORLD_HPP
#define WORLD_HPP

#include "CommandQueue.hpp"
#include "ObjectContext.hpp"
#include "LocationEvent.hpp"
#include "State.hpp"
#include "SoundPlayer.hpp"
#include "Entities/Spaceship.hpp"
#include "ResourcesID.hpp"
#include "SceneNode.hpp"
#include "Profile.hpp"
#include "AABBTree/AABBTree.hpp"
#include "ParticleSystem/ParticleNode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <array>
#include <unordered_map>
#include <list>

class World
{
    public:
        explicit                        World(State::Context context);
                                        ~World();
        CommandQueue&                   getCommandQueue();
        void                            update(sf::Time);
        void                            draw();
        sf::FloatRect                   getViewBounds() const;
        sf::FloatRect                   getBattlefieldBounds() const;
        void                            placeOnLayer(SceneNode::Ptr, Category::Type layer);
        bool                            hasPlayerReachedEnd() const;
        bool                            hasAlivePlayer() const;
        std::unordered_map<int, int>&   getPlayersScoresMap();
        Spaceship*                      addSpaceship(int id);
        void                            setScrollingSpeed(float);

    private:
        void    buildWorld();
        void    adaptPlayersVelocity();
        void    addSpawnPoint(float x, float y, int spaceshipID);
        void    sortSpawnPoints();
        void    spawnEnemies();
        void    guideHomingMissiles();
        void    adaptPlayersPosition();
        void    checkCollisions();
        void    destroyEntitiesOutsideView();
        void    updateSounds();
        void    updateScore();
        void    updateEvents();
        void    removeDanglingPointers();
        void    loadLevelData();
        void    createAABBTree();

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
            int spaceshipID;
        };

        sf::RenderTarget&   mTarget;
        TextureHolder&      mTextures;
        FontHolder&         mFonts;
        SoundPlayer&        mSoundPlayer;
        MusicPlayer&        mMusicPlayer;
        Profile&            mProfile;
        SceneNode           mSceneGraph;
        SceneNode           mUIGraph;
        CommandQueue        mCommandQueue;
        sf::View            mView;
        sf::FloatRect       mWorldBounds;
        sf::Vector2f        mPlayerSpawnPosition;
        float               mScrollingSpeed;
        ParticleNode*       mParticleNode;
        sf::Text            mScore;
        bool                mIsDeleting;
        ObjectContext       mObjectContext;

        std::vector<Spaceship*>             mPlayerSpaceships;
        std::vector<Spaceship*>             mEnemies;
        std::vector<SpawnPoint>             mSpawnPoints;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        std::unordered_map<int, Entity*>    mCollidables;
        AABBTree                            mCollisionTree;
        std::unordered_map<int, int>        mPlayersScores;
        std::vector<LocationEvent>          mEvents;
};

#endif // WORLD_HPP
