#ifndef QUADTREE_H
#define QUADTREE_H

#include"Bound.h"

class QuadTreeNode
{
public:
	QuadTreeNode(float _centerX, float _centerZ, float _halfSize, int _depth = 0);
	void EvaluateVisibility();
private:
	void CalculateBoundingBox();
	void SetInvisible();
public:
	float centerX;
	float centerZ;
	float halfSize;

	int depth;
	bool isLeaf;
	bool isInFrustum;

	QuadTreeNode *leftTop;
	QuadTreeNode *rightTop;
	QuadTreeNode *leftBottom;
	QuadTreeNode *rightBottom;

	BoundingBox boundingBox;

};

#endif