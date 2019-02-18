#ifndef WORLD_HPP
#define WORLD_HPP

#include "CommandQueue.hpp"
#include "State.hpp"
#include "SoundPlayer.hpp"
#include "Entities/Aircraft.hpp"
#include "ResourcesID.hpp"
#include "SceneNode.hpp"
#include "Profile.hpp"
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
        void                            addCollidable(Entity*);
        void                            removeCollidable(Entity*);
        ParticleNode&                   getParticleNode() const;
        SoundPlayer&                    getSoundPlayer() const;
        void                            placeOnLayer(SceneNode::Ptr, Category::Type layer);
        bool                            hasPlayerReachedEnd() const;
        bool                            hasAlivePlayer() const;
        std::unordered_map<int, int>&   getPlayersScoresMap();
        Aircraft*                       addAircraft(int id);

    private:
        void    buildWorld();
        void    adaptPlayersVelocity();
        void    addSpawnPoint(float x, float y, Aircraft::Type);
        void    sortSpawnPoints();
        void    spawnEnemies();
        void    guideHomingMissiles();
        void    adaptPlayersPosition();
        void    checkCollisions();
        void    destroyEntitiesOutsideView();
        void    updateSounds();
        void    updateScore();
        void    removeDanglingPointers();
        void    loadLevelData();

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
            Aircraft::Type type; ///Change to int
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

        std::vector<Aircraft*>              mPlayerAircrafts;
        std::vector<SpawnPoint>             mSpawnPoints;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        std::list<Entity*>                  mCollidablesList;
        std::unordered_map<int, int>        mPlayersScores;
};

#endif // WORLD_HPP
