#ifndef AABBTREE_HPP
#define AABBTREE_HPP

#include "AABBNode.hpp"
#include <memory>
#include <vector>

class AABBTree
{
    public:
        explicit            AABBTree(unsigned int initialSize);
        void                insertEntity(const AABB& entity);
        std::vector<int>    queryOverlaps(const AABB& entity) const;
        void                clear();

    private:
        int                 allocateNode();
        void                fixUpwardsTree(int currentNodeIndex);

        int                     mRootNodeIndex;
        int                     mAllocatedNodeCount;
        int                     mNodeCapacity;
        int                     mInitialSize;
        std::vector<AABBNode>   mNodes;
};

#endif // AABBTREE_HPP
