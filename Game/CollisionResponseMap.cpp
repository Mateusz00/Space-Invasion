#include "CollisionResponseMap.hpp"
#include "Entities/Entity.hpp"

std::unordered_map<Category::Type, CollisionResponseMap::ResponseMap, std::hash<int>> CollisionResponseMap::mCollisionResponsesMap;

void CollisionResponseMap::addResponse(Category::Type type1, Category::Type type2, Response response)
{
    (mCollisionResponsesMap[type1])[type2] = response;
}

void CollisionResponseMap::useResponse(Entity* entity1, Entity* entity2)
{
    Category::Type type1 = entity1->getCategories();
    Category::Type type2 = entity2->getCategories();

    auto foundPos = mCollisionResponsesMap.find(type1);
    Category::Type searchType;

    if(foundPos != mCollisionResponsesMap.end())
    {
        searchType = type2;
    }
    else if((foundPos = mCollisionResponsesMap.find(type2)) != mCollisionResponsesMap.end())
    {
        searchType = type1;
        std::swap(entity1, entity2);
    }
    else
    {
        return;
    }

    auto responsePos = foundPos->second.find(searchType);

    if(responsePos != foundPos->second.end())
        (responsePos->second)(*entity1, *entity2);
}

bool CollisionResponseMap::checkForResponse(Category::Type type1, Category::Type type2)
{
    auto foundPos = mCollisionResponsesMap.find(type1);
    Category::Type searchType;

    if(foundPos != mCollisionResponsesMap.end())
        searchType = type2;
    else if((foundPos = mCollisionResponsesMap.find(type2)) != mCollisionResponsesMap.end())
        searchType = type1;
    else
        return false;

    auto responsePos = foundPos->second.find(searchType);

    if(responsePos != foundPos->second.end())
        return true;
    else
        return false;
}
