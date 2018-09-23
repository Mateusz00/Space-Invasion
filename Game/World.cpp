#include "World.hpp"
#include "Utility.hpp"
#include "SpriteNode.hpp"
#include "Entities/AmmoNode.hpp"
#include <memory>
#include <cmath>
#include <limits>

World::World(sf::RenderTarget& target, TextureHolder& textures, FontHolder& fonts)
    : mTarget(target),
      mTextures(textures),
      mFonts(fonts),
      mPlayerAircraft(nullptr),
      mView(target.getDefaultView()),
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

    guideHomingMissiles();

    while(!mCommandQueue.isEmpty())
        mSceneGraph.executeCommand(mCommandQueue.pop(), dt);

    adaptPlayersVelocity();
    spawnEnemies();
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayersPosition();
}

void World::draw()
{
    mTarget.setView(mView);
    mTarget.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
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

    std::unique_ptr<Aircraft> playerAircraft(new Aircraft(Aircraft::Ally, mTextures, mFonts));
    playerAircraft->setPosition(mPlayerSpawnPosition); // Change to viewCenter
    playerAircraft->setIdentifier(0);
    mPlayerAircraft = playerAircraft.get();
    mSceneLayers[UpperAir]->attachChild(std::move(playerAircraft));

    std::unique_ptr<AmmoNode> ammoNode(new AmmoNode(*mPlayerAircraft, mTextures, mFonts, mView));
    mSceneGraph.attachChild(std::move(ammoNode));
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
	addSpawnPoint(300.f, 5800.f, Aircraft::Enemy);
	addSpawnPoint(500.f, 5700.f, Aircraft::Enemy);

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
    std::sort(mSpawnPoints.begin(), mSpawnPoints.end(), [](SpawnPoint first, SpawnPoint second)
	{
		return first.y < second.y;
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

		std::unique_ptr<Aircraft> enemyAircraft(new Aircraft(spawn.type, mTextures, mFonts));
		enemyAircraft->setPosition(spawn.x, spawn.y);
		mActiveEnemies.push_back(enemyAircraft.get());
		mSceneLayers[UpperAir]->attachChild(std::move(enemyAircraft));

		mSpawnPoints.pop_back();
	}
}

void World::guideHomingMissiles()
{
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

	mCommandQueue.push(homingCommand);
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
