#ifndef AABBNODE_HPP
#define AABBNODE_HPP

#include "AABB.hpp"

#define NULL_NODE -1

struct AABBNode
{
    AABB aabb;
	int parentNodeIndex = NULL_NODE;
	int leftNodeIndex   = NULL_NODE;
	int rightNodeIndex  = NULL_NODE;

	bool isLeaf() const { return (leftNodeIndex == NULL_NODE && rightNodeIndex == NULL_NODE); }
};

#endif // AABBNODE_HPP
