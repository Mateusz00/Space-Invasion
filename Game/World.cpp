#include "World.hpp"
#include "Utility.hpp"
#include "SpriteNode.hpp"
#include "Entities/AmmoNode.hpp"
#include <memory>
#include <cmath>
#include <limits>
#include <iostream>

World::World(State::Context context)
    : mTarget(context.window),
      mTextures(context.textures),
      mFonts(context.fonts),
      mSoundPlayer(context.sounds),
      mMusicPlayer(context.music),
      mPlayerAircraft(nullptr),
      mView(mTarget.getDefaultView()),
      mWorldBounds(0.f, 0.f, mView.getSize().x, 7000.f),
      mPlayerSpawnPosition(mView.getSize().x / 2, mWorldBounds.height - mView.getSize().y / 2.f),
      mScrollingSpeed(-40.f)
{
    buildWorld();
    initializeSpawnPoints();
    mView.setCenter(mPlayerSpawnPosition);
}

void World::update(sf::Time dt)
{
    mView.move(0.f, mScrollingSpeed * dt.asSeconds());

    if(mPlayerAircraft)
        mPlayerAircraft->setVelocity(0.f, 0.f);

    destroyEntitiesOutsideView();
    guideHomingMissiles();

    while(!mCommandQueue.isEmpty())
        mSceneGraph.executeCommand(mCommandQueue.pop(), dt);

    adaptPlayersVelocity();
    checkCollisions();

    spawnEnemies();
    mSceneGraph.update(dt, mCommandQueue);
    mSceneGraph.removeWrecks();
    adaptPlayersPosition();
    mUIGraph.update(dt, mCommandQueue);
    updateSounds();
}

void World::draw()
{
    mTarget.setView(mView);
    mTarget.draw(mSceneGraph);
    mTarget.setView(mTarget.getDefaultView());
    mTarget.draw(mUIGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::addCollidable(Entity* entity)
{
    mCollidablesList.emplace_back(entity);
    entity->getPositionOnList() = --(mCollidablesList.end());
}

void World::removeCollidable(Entity* entity)
{
    mCollidablesList.erase(entity->getPositionOnList());
    std::cout << "REMOVAL" << std::endl;
}

ParticleNode& World::getParticleNode() const
{
    return *mParticleNode;
}

SoundPlayer& World::getSoundPlayer() const
{
    return mSoundPlayer;
}

void World::placeOnLayer(SceneNode::Ptr node, Category::Type layer)
{
    mSceneLayers[layer]->attachChild(std::move(node));
}

void World::buildWorld()
{
    for(int i=0; i < LayerCount; ++i)
    {
        Category::Type category = (i == LowerAir) ? Category::AirLayer : Category::None; // LowerAir for particles, UpperAir for collidables
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& background = mTextures.get(Textures::Background);
    background.setRepeated(true);
    sf::IntRect backgroundRect(0, 0, 1024, 7000);

    std::unique_ptr<SpriteNode> galaxyBackground(new SpriteNode(background, backgroundRect));
    galaxyBackground->setPosition(0.f, 0.f);
    mSceneLayers[Background]->attachChild(std::move(galaxyBackground));

    std::unique_ptr<Aircraft> playerAircraft(new Aircraft(Aircraft::Ally, mTextures, mFonts, *this));
    playerAircraft->setPosition(mPlayerSpawnPosition); // Change to viewCenter
    playerAircraft->setIdentifier(0);
    mPlayerAircraft = playerAircraft.get();
    mSceneLayers[UpperAir]->attachChild(std::move(playerAircraft));

    std::unique_ptr<AmmoNode> ammoNode(new AmmoNode(*mPlayerAircraft, mTextures, mFonts, mView));
    mUIGraph.attachChild(std::move(ammoNode));

    std::unique_ptr<ParticleNode> particleNode(new ParticleNode(mTextures));
    mParticleNode = particleNode.get();
    mSceneLayers[LowerAir]->attachChild(std::move(particleNode));
}

void World::adaptPlayersVelocity()
{
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    if(velocity.x != 0.f && velocity.y != 0.f)
        mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

    mPlayerAircraft->accelerate(0.f, mScrollingSpeed);
}

void World::initializeSpawnPoints()
{
	addSpawnPoint(300.f, 5900.f, Aircraft::Enemy);
	addSpawnPoint(500.f, 5700.f, Aircraft::Enemy);
    addSpawnPoint(300.f, 5150.f, Aircraft::Enemy);
	addSpawnPoint(500.f, 5250.f, Aircraft::Enemy);

	sortSpawnPoints();
	// TODO: Add more later
}

void World::addSpawnPoint(float x, float y, Aircraft::Type type)
{
    SpawnPoint spawnPoint{x, y, type};
	mSpawnPoints.push_back(std::move(spawnPoint));
}

void World::sortSpawnPoints()
{
    std::sort(mSpawnPoints.begin(), mSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getViewBounds();
    bounds.height += 300.f;
    bounds.top -= 300.f;
    return bounds;
}

sf::FloatRect World::getViewBounds() const
{
    sf::FloatRect bounds(mView.getCenter() - mView.getSize() / 2.f, mView.getSize());
    return bounds;
}

void World::spawnEnemies()
{
	while(!mSpawnPoints.empty() && mSpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mSpawnPoints.back();

		std::unique_ptr<Aircraft> enemyAircraft(new Aircraft(spawn.type, mTextures, mFonts, *this));
		enemyAircraft->setPosition(spawn.x, spawn.y);
		mSceneLayers[UpperAir]->attachChild(std::move(enemyAircraft));

		mSpawnPoints.pop_back();
	}
}

void World::guideHomingMissiles()
{
    Command enemyCollector;
    enemyCollector.mCategories.push_back(Category::EnemyAircraft);
    enemyCollector.mAction = castFunctor<Aircraft>([this](Aircraft& enemy, sf::Time dt)
    {
        if(!enemy.isMarkedForRemoval())
            mActiveEnemies.push_back(&enemy);
    });

    Command homingCommand;
	homingCommand.mCategories.push_back(Category::AlliedProjectile);
	homingCommand.mAction = castFunctor<Projectile>([this](Projectile& missile, sf::Time dt)
	{
		if(!missile.isGuided())
			return;

		float smallestDistance = std::numeric_limits<float>::max();
		Aircraft* closestEnemy = nullptr;

		for(const auto& enemy : mActiveEnemies)
        {
            float enemyDistance = vectorLength(missile.getWorldPosition() - enemy->getWorldPosition());
            if(enemyDistance < smallestDistance)
            {
                closestEnemy = enemy;
                smallestDistance = enemyDistance;
            }
        }

        if(closestEnemy)
            missile.guideTowards(closestEnemy->getWorldPosition());
	});

    mCommandQueue.push(enemyCollector);
	mCommandQueue.push(homingCommand);
	mActiveEnemies.clear();
}

void World::adaptPlayersPosition()
{
    sf::FloatRect viewBounds = getViewBounds();
	const sf::Vector2f distanceFromBorder(37.5f, 26.f);
    sf::Vector2f playerPosition = mPlayerAircraft->getPosition();

    if(playerPosition.x < viewBounds.left + distanceFromBorder.x)
        playerPosition.x = viewBounds.left + distanceFromBorder.x;
    else if(playerPosition.x > viewBounds.left + viewBounds.width - distanceFromBorder.x)
        playerPosition.x = viewBounds.left + viewBounds.width - distanceFromBorder.x;

    if(playerPosition.y < viewBounds.top + distanceFromBorder.y)
        playerPosition.y = viewBounds.top + distanceFromBorder.y;
    else if(playerPosition.y > viewBounds.top + viewBounds.height - distanceFromBorder.y)
        playerPosition.y = viewBounds.top + viewBounds.height - distanceFromBorder.y;

    mPlayerAircraft->setPosition(playerPosition);
}

void World::checkCollisions()
{
    for(auto nodeA = mCollidablesList.begin(); nodeA != (--mCollidablesList.end()); ++nodeA) // Don't check last node
    {
        auto nodePastA = nodeA;
        ++nodePastA; // Don't check two same nodes

        for(; nodePastA != mCollidablesList.end(); ++nodePastA)
        {
            if(collision(**nodeA, **nodePastA))
            {
                (**nodeA).onCollision(**nodePastA);
                (**nodePastA).onCollision(**nodeA);
            }
        }
    }
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.mCategories.push_back(Category::EnemyProjectile);
    command.mCategories.push_back(Category::AlliedProjectile);
    command.mCategories.push_back(Category::EnemyAircraft);
    command.mAction = castFunctor<Entity>([this](Entity& object, sf::Time dt)
    {
        if(!getBattlefieldBounds().intersects(object.getBoundingRect()))
            object.removeEntity();
    });
    mCommandQueue.push(command);
}

void World::updateSounds()
{
    mSoundPlayer.removeStoppedSounds();
    mSoundPlayer.setListener(mPlayerAircraft->getWorldPosition());
}
