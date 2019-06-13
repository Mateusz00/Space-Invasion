#include "AABBTree.hpp"
#include <stack>

AABBTree::AABBTree(unsigned int initialSize)
    : mRootNodeIndex(NULL_NODE),
      mAllocatedNodeCount(0),
      mNodeCapacity(initialSize),
      mInitialSize(initialSize)
{
	mNodes.resize(initialSize);
}

/// @return Index of allocated node
int AABBTree::allocateNode()
{
	// Allocate bigger array if necessary
	if(mAllocatedNodeCount == mNodeCapacity)
	{
		mNodeCapacity *= 2;
		mNodes.resize(mNodeCapacity);
	}

	return mAllocatedNodeCount++;
}

void AABBTree::insertEntity(const AABB& entity)
{
	int newLeafNodeIndex = allocateNode();
	mNodes[newLeafNodeIndex].aabb = entity;

	if(mRootNodeIndex == NULL_NODE)
	{
		mRootNodeIndex = newLeafNodeIndex;
		return;
	}

	// Insert node in best place based on surface area
	int currentNodeIndex = mRootNodeIndex;
	AABBNode& newLeafNode = mNodes[newLeafNodeIndex];
	while(!mNodes[currentNodeIndex].isLeaf())
	{
		const AABBNode& currentNode = mNodes[currentNodeIndex];
		const AABBNode& leftNode    = mNodes[currentNode.leftNodeIndex];
		const AABBNode& rightNode   = mNodes[currentNode.rightNodeIndex];

		sf::FloatRect combinedAABB = mergeRects(currentNode.aabb.rect, newLeafNode.aabb.rect);

		float newParentNodeCost = getSurfaceArea(combinedAABB);
		float minimumPushDownCost = getSurfaceArea(combinedAABB) - getSurfaceArea(currentNode.aabb.rect);
        float costLeft, costRight;

		// Calculate costs to find in which direction it's better to descend
		if(leftNode.isLeaf())
			costLeft = getSurfaceArea(mergeRects(newLeafNode.aabb.rect, leftNode.aabb.rect)) + minimumPushDownCost;
		else
			costLeft = getSurfaceArea(mergeRects(newLeafNode.aabb.rect, leftNode.aabb.rect))
                       - getSurfaceArea(leftNode.aabb.rect) + minimumPushDownCost;

		if(rightNode.isLeaf())
			costRight = getSurfaceArea(mergeRects(newLeafNode.aabb.rect, rightNode.aabb.rect)) + minimumPushDownCost;
		else
			costRight = getSurfaceArea(mergeRects(newLeafNode.aabb.rect, rightNode.aabb.rect))
                        - getSurfaceArea(rightNode.aabb.rect) + minimumPushDownCost;

		if(newParentNodeCost < costLeft && newParentNodeCost < costRight)
			break;

		if(costLeft < costRight)
			currentNodeIndex = currentNode.leftNodeIndex;
		else
			currentNodeIndex = currentNode.rightNodeIndex;
	}

	// currentNodeIndex contains index of AABB that it's best to pair with new node
	AABBNode& leafSibling = mNodes[currentNodeIndex];
	int newParentIndex = allocateNode();
	AABBNode& newParent = mNodes[newParentIndex];

	// Fix linking between nodes
    int oldParentIndex = leafSibling.parentNodeIndex;
	newParent.parentNodeIndex = oldParentIndex;
    newLeafNode.parentNodeIndex = newParentIndex;
	leafSibling.parentNodeIndex = newParentIndex;
    newParent.leftNodeIndex = currentNodeIndex;
	newParent.rightNodeIndex = newLeafNodeIndex;

	// If oldParent was root node then update root node index of tree
	if(oldParentIndex == NULL_NODE)
	{
		mRootNodeIndex = newParentIndex;
	}
	else
	{
		// If old parent wasn't root then we need to update it's left/right node index so that it will be index of new parent
		if(mNodes[oldParentIndex].leftNodeIndex == currentNodeIndex)
			mNodes[oldParentIndex].leftNodeIndex = newParentIndex;
		else
			mNodes[oldParentIndex].rightNodeIndex = newParentIndex;
	}

	newParent.aabb.rect = mergeRects(newLeafNode.aabb.rect, leafSibling.aabb.rect);
	fixUpwardsTree(newLeafNode.parentNodeIndex);
}

std::vector<int> AABBTree::queryOverlaps(const AABB& entity) const
{
	std::vector<int> collisions;
	std::stack<int> traversalStack;
	sf::FloatRect testAABB = entity.rect;

	traversalStack.push(mRootNodeIndex);
	while(!traversalStack.empty())
	{
		int currentNodeIndex = traversalStack.top();
		traversalStack.pop();

		if(currentNodeIndex == NULL_NODE)
		    continue;

		const AABBNode& node = mNodes[currentNodeIndex];
		if(node.aabb.rect.intersects(testAABB))
		{
			if(node.isLeaf() && node.aabb.entityID != entity.entityID)
			{
				collisions.emplace_back(node.aabb.entityID);
			}
			else
			{
				traversalStack.push(node.leftNodeIndex);
				traversalStack.push(node.rightNodeIndex);
			}
		}
	}

	return collisions;
}

/// Updates all parent's rects of currentNodeIndex
void AABBTree::fixUpwardsTree(int currentNodeIndex)
{
	while(currentNodeIndex != NULL_NODE)
	{
		AABBNode& currentNode     = mNodes[currentNodeIndex];
		const AABBNode& leftNode  = mNodes[currentNode.leftNodeIndex];
		const AABBNode& rightNode = mNodes[currentNode.rightNodeIndex];

		currentNode.aabb.rect = mergeRects(leftNode.aabb.rect, rightNode.aabb.rect);
		currentNodeIndex = currentNode.parentNodeIndex;
	}
}

void AABBTree::clear()
{
    mNodes.clear();
    mNodes.resize(mInitialSize);
    mAllocatedNodeCount = 0;
    mRootNodeIndex = NULL_NODE;
}
