#include "CollisionResponseMap.hpp"
#include "Entities/Entity.hpp"

std::unordered_map<Category::Type, CollisionResponseMap::ResponseMap, std::hash<int>> CollisionResponseMap::mCollisionResponsesMap;

void CollisionResponseMap::addResponse(Category::Type type1, Category::Type type2, Response response)
{
    auto typePair = std::minmax(type1, type2);
    (mCollisionResponsesMap[typePair.first])[typePair.second] = response;
}

void CollisionResponseMap::useResponse(Entity* entity1, Entity* entity2)
{
    Category::Type type1 = entity1->getCategories();
    Category::Type type2 = entity2->getCategories();
    auto typePair = std::minmax(type1, type2);
    auto foundPos = mCollisionResponsesMap.find(typePair.first);

    if(typePair.first == type2)
        std::swap(entity1, entity2);

    if(foundPos != mCollisionResponsesMap.end())
    {
        auto responsePos = foundPos->second.find(typePair.second);

        if(responsePos != foundPos->second.end())
            (responsePos->second)(*entity1, *entity2);
    }
}

bool CollisionResponseMap::checkForResponse(Category::Type type1, Category::Type type2)
{
    auto typePair = std::minmax(type1, type2);
    auto foundPos = mCollisionResponsesMap.find(typePair.first);

    if(foundPos != mCollisionResponsesMap.end())
    {
        auto responsePos = foundPos->second.find(typePair.second);

        if(responsePos != foundPos->second.end())
            return true;
    }

    return false;
}
