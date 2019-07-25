#include "World.hpp"
#include "Attack.hpp"
#include "Utility.hpp"
#include "SpriteNode.hpp"
#include "Entities/AmmoNode.hpp"
#include "Entities/ScoreNode.hpp"
#include "Exceptions/XMLParseException.hpp"
#include "CollisionResponseMap.hpp"
#include <memory>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <limits>
#include <pugixml.hpp>

World::World(State::Context context)
    : mTarget(context.window),
      mTextures(context.textures),
      mFonts(context.fonts),
      mSoundPlayer(context.sounds),
      mMusicPlayer(context.music),
      mProfile(context.profile),
      mView(mTarget.getDefaultView()),
      mScrollingSpeed(-32.f),
      mIsDeleting(false),
      mCollisionTree(4)
{
    Entity::resetIDs();
    loadLevelData();
    buildWorld();
    mView.setCenter(mPlayerSpawnPosition);

    mObjectContext.commandQueue = &mCommandQueue;
    mObjectContext.events = &mEvents;
    mObjectContext.soundPlayer = &mSoundPlayer;
}

World::~World()
{
    mIsDeleting = true;
    mSoundPlayer.setListener(sf::Vector3f(0.f, 0.f, -1.f));
}

void World::update(sf::Time dt)
{
    mView.move(0.f, mScrollingSpeed * dt.asSeconds());

    for(auto& playerSpaceship : mPlayerSpaceships)
        playerSpaceship->setVelocity(0.f, 0.f);

    destroyEntitiesOutsideView();
    guideHomingMissiles();
    createAABBTree();

    while(!mCommandQueue.isEmpty())
        mSceneGraph.executeCommand(mCommandQueue.pop(), dt);

    adaptPlayersVelocity();
    checkCollisions();

    spawnEnemies();
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayersPosition();
    updateSounds();
    updateEvents();
    mUIGraph.update(dt, mCommandQueue);

    removeDanglingPointers(); // Erase pointers of destroyed players spaceships as remove wrecks removes only entities
    mSceneGraph.removeWrecks();
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

void World::placeOnLayer(SceneNode::Ptr node, Category::Type layer)
{
    mSceneLayers[layer]->attachChild(std::move(node));
}

bool World::hasPlayerReachedEnd() const
{
    for(auto& playerSpaceship : mPlayerSpaceships)
    {
        if(!mWorldBounds.contains(playerSpaceship->getPosition()))
           return true;
    }

    return false;
}

bool World::hasAlivePlayer() const
{
    return mPlayerSpaceships.size() > 0;
}

std::unordered_map<int, int> World::getPlayersScoresMap()
{
    std::unordered_map<int, int> scores;

    Command scoresGetter;
    scoresGetter.mCategories = Category::ScoreNode;
    scoresGetter.mAction = castFunctor<ScoreNode>([this, &scores](ScoreNode& scoreNode, sf::Time)
    {
        scores = scoreNode.getPlayersScoresMap();
    });
    mUIGraph.executeCommand(scoresGetter, sf::Time::Zero);

    return scores;
}

Spaceship* World::addSpaceship(int id)
{
    std::unique_ptr<Spaceship> playerSpaceship(new Spaceship(0, mTextures, mFonts, mObjectContext, mEnemies, id));
    playerSpaceship->setPosition(mPlayerSpawnPosition.x - 50.f + 100.f*(id%2), mPlayerSpawnPosition.y);
    mPlayerSpaceships.emplace_back(playerSpaceship.get());

    std::unique_ptr<AmmoNode> ammoNode(new AmmoNode(id, mTextures, mFonts, mView));
    ammoNode->setMargin(2.f);
    ammoNode->align(static_cast<AmmoNode::Align>(id%2));
    mUIGraph.attachChild(std::move(ammoNode));
    mSceneLayers[UpperAir]->attachChild(std::move(playerSpaceship));
    return mPlayerSpaceships.back();
}

void World::setScrollingSpeed(float speed)
{
    mScrollingSpeed = speed;
}

void World::buildWorld()
{
    for(int i=0; i < LayerCount; ++i)
    {
        Category::Type category = (i == LowerAir) ? Category::AirLayer : Category::None;
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& background = mTextures.get(Textures::Background);
    background.setRepeated(true);
    sf::IntRect backgroundRect(mWorldBounds);
    backgroundRect.height += static_cast<int>(mView.getSize().y); // Make background higher by view size to prevent user from seeing black space

    std::unique_ptr<SpriteNode> galaxyBackground(new SpriteNode(background, backgroundRect));
    galaxyBackground->setPosition(mWorldBounds.left, mWorldBounds.top - mView.getSize().y); // Position it higher by view size to prevent user from seeing black space
    mSceneLayers[Background]->attachChild(std::move(galaxyBackground));

    sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
    finishTexture.setRepeated(true);
    sf::IntRect finishLineRect(0, 0, mTarget.getSize().x, 100);

    std::unique_ptr<SpriteNode> finishLine(new SpriteNode(finishTexture, finishLineRect));
    finishLine->setPosition(0.f, -50.f);
    mSceneLayers[Background]->attachChild(std::move(finishLine));

    std::unique_ptr<ParticleNode> particleNode(new ParticleNode(mTextures));
    mObjectContext.particleNode = particleNode.get();
    mSceneLayers[LowerAir]->attachChild(std::move(particleNode));

    mUIGraph.attachChild(std::make_unique<ScoreNode>(mFonts));
}

void World::adaptPlayersVelocity()
{
    sf::Vector2f velocity;

    for(auto& playerSpaceship : mPlayerSpaceships)
    {
        velocity = playerSpaceship->getVelocity();

        if(velocity.x != 0.f && velocity.y != 0.f)
            playerSpaceship->setVelocity(velocity / std::sqrt(2.f));

        playerSpaceship->trySpeedBoost();
        playerSpaceship->accelerate(0.f, mScrollingSpeed);
    }
}

void World::loadLevelData()
{
    using namespace pugi;

    // Load xml file, throw errors
    std::string filename("level" + toString(mProfile.getCurrentLevel()+1) + ".xml"); // Level with id=0 is name level1.xml
    xml_document doc;
    xml_parse_result result = doc.load_file(("Levels/" + filename).c_str());
    if(!result)
        throw XMLParseException(result, filename);

    // Load spawn points of enemies
    xml_node spawnPoints = doc.child("spawnpoints");
    for(xml_node spawnPoint : spawnPoints.children())
    {
        float x = spawnPoint.attribute("x").as_float();
        float y = spawnPoint.attribute("y").as_float();
        int id = spawnPoint.attribute("enemyID").as_int(1);

        addSpawnPoint(x, y, id);
    }
    sortSpawnPoints();

    // Load data about level length etc.
    float length;
    xml_node levelData = doc.child("leveldata");
    std::string lengthString = levelData.child_value("levelwidth");
    std::runtime_error lengthExcepetion("In level" + toString(mProfile.getCurrentLevel()+1) + ".xml: Value can't be empty or zero");

    if(lengthString.empty())
        throw lengthExcepetion;

    std::istringstream lengthStream(lengthString);
    lengthStream >> length;

    if(length <= 0.f)
        throw lengthExcepetion;

    mWorldBounds = sf::FloatRect(0.f, 0.f, mView.getSize().x, length);
    mPlayerSpawnPosition = sf::Vector2f(mView.getSize().x / 2, mWorldBounds.height - mView.getSize().y / 2.f);

    // Load events
    xml_node eventData = doc.child("events");
    for(xml_node event : eventData.children())
    {
        int id = event.attribute("id").as_int(-1);
        float x = event.attribute("x").as_float(-1);
        float y = event.attribute("y").as_float(-1);
        LocationEvent newEvent(sf::Vector2f(x, y), static_cast<LocationEvent::Event>(id));

        event.remove_attribute("id");
        event.remove_attribute("x");
        event.remove_attribute("y");

        if(id < 0)
            throw std::runtime_error("Event ID not found or less than zero!(Level" +
                                      toString(mProfile.getCurrentLevel()+1) + ".xml)");

        for(xml_attribute eventAttribute : event.attributes())
            newEvent.addParameter(eventAttribute.name(), eventAttribute.value());

        mEvents.push(std::move(newEvent));
    }
}

void World::addSpawnPoint(float x, float y, int spaceshipID)
{
    SpawnPoint spawnPoint{x, y, spaceshipID};
    mSpawnPoints.push_back(std::move(spawnPoint));
}

void World::sortSpawnPoints()
{
    std::sort(mSpawnPoints.begin(), mSpawnPoints.end(), [](SpawnPoint& lhs, SpawnPoint& rhs)
    {
        return lhs.y < rhs.y;
    });
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getViewBounds();
    bounds.height += 200.f;
    bounds.top -= 200.f;
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
        SpawnPoint& spawn = mSpawnPoints.back();

        std::unique_ptr<Spaceship> enemySpaceship(new Spaceship(spawn.spaceshipID, mTextures, mFonts, mObjectContext, mPlayerSpaceships));
        enemySpaceship->setPosition(spawn.x, spawn.y);
        mSceneLayers[UpperAir]->attachChild(std::move(enemySpaceship));

        mSpawnPoints.pop_back();
    }
}

void World::guideHomingMissiles()
{
    Command mEnemyCollector;
    mEnemyCollector.mCategories = Category::EnemySpaceship;
    mEnemyCollector.mAction = castFunctor<Spaceship>([this](Spaceship& target, sf::Time dt)
    {
        if(!target.isMarkedForRemoval())
            mEnemies.emplace_back(&target);
    });

    mEnemies.clear();
    mCommandQueue.push(mEnemyCollector);
}

void World::adaptPlayersPosition()
{
    sf::FloatRect viewBounds = getViewBounds();
    const sf::Vector2f distanceFromBorder(37.5f, 26.f);
    sf::Vector2f playerPosition;

    for(auto& playerSpaceship : mPlayerSpaceships)
    {
        playerPosition = playerSpaceship->getPosition();

        if(playerPosition.x < viewBounds.left + distanceFromBorder.x)
            playerPosition.x = viewBounds.left + distanceFromBorder.x;
        else if(playerPosition.x > viewBounds.left + viewBounds.width - distanceFromBorder.x)
            playerPosition.x = viewBounds.left + viewBounds.width - distanceFromBorder.x;

        if(playerPosition.y < viewBounds.top + distanceFromBorder.y)
            playerPosition.y = viewBounds.top + distanceFromBorder.y;
        else if(playerPosition.y > viewBounds.top + viewBounds.height - distanceFromBorder.y)
            playerPosition.y = viewBounds.top + viewBounds.height - distanceFromBorder.y;

        playerSpaceship->setPosition(playerPosition);
    }
}

void World::checkCollisions()
{
    // Collision detection
    std::set<std::pair<int, int>> collisionPairs;
    for(const auto& collidablePair : mCollidables)
    {
        std::vector<int> collisions = mCollisionTree.queryOverlaps(AABB{collidablePair.second.rect, collidablePair.first});
        for(int id : collisions)
            collisionPairs.emplace(std::minmax(id, collidablePair.first));
    }

    // Collision response
    for(const auto& collisionPair : collisionPairs)
        CollisionResponseMap::useResponse(mCollidables.at(collisionPair.first).collidable, mCollidables.at(collisionPair.second).collidable);
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.mCategories = static_cast<Category::Type>(Category::Attack + Category::EnemySpaceship);
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
    mSoundPlayer.setListener(mPlayerSpaceships[0]->getWorldPosition());
}

void World::updateEvents()
{
    while(!mEvents.isEmpty() && getBattlefieldBounds().contains(mEvents.front().getTriggerLocation()))
        mEvents.pop().handle(*this);
}

void World::removeDanglingPointers()
{
    auto firstToRemove = std::remove_if(mPlayerSpaceships.begin(), mPlayerSpaceships.end(), std::mem_fn(&Spaceship::isMarkedForRemoval));
    mPlayerSpaceships.erase(firstToRemove, mPlayerSpaceships.end());
}

void World::createAABBTree()
{
    // Get list and create AABB Tree of entities that can collide
    Command collidablesCollector;
    collidablesCollector.mCategories = Category::Collidable;
    collidablesCollector.mAction = castFunctor<Entity>([this](Entity& entity, sf::Time)
    {
        int id = entity.getEntityID();
        AABB aabb{sf::FloatRect(entity.getBoundingRect()), id};
        mCollidables.emplace(id, CollidableData{&entity, aabb.rect});
        mCollisionTree.insertEntity(aabb);
    });
    Command projectileCollector;
    projectileCollector.mCategories = Category::Attack;
    projectileCollector.mAction = castFunctor<Attack>([this](Attack& attack, sf::Time)
    {
        std::vector<Projectile*> projectiles = attack.getProjectiles();
        for(Projectile* projectile : projectiles)
        {
            int id = projectile->getEntityID();
            AABB aabb{sf::FloatRect(projectile->getBoundingRect()), id};
            mCollidables.insert(std::make_pair(id, CollidableData{projectile, aabb.rect}));
            mCollisionTree.insertEntity(aabb);
        }
    });

    mCollidables.clear();
    mCollisionTree.clear();
    mCommandQueue.push(collidablesCollector);
    mCommandQueue.push(projectileCollector);
}
