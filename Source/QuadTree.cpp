#include"QuadTree.h"
#include"Scene.h"

QuadTreeNode::QuadTreeNode(float _centerX, float _centerZ, float _halfSize, int _depth)
{
	centerX = _centerX;
	centerZ = _centerZ;
	halfSize = _halfSize;
	depth = _depth;

	CalculateBoundingBox();

	if(depth > 0){
		isLeaf = false;
		leftTop = new QuadTreeNode(centerX - halfSize / 2.0f, centerZ + halfSize / 2.0f, halfSize / 2.0f, depth - 1);
		rightTop = new QuadTreeNode(centerX + halfSize / 2.0f, centerZ + halfSize / 2.0f, halfSize / 2.0f, depth - 1);
		leftBottom = new QuadTreeNode(centerX - halfSize / 2.0f, centerZ - halfSize / 2.0f, halfSize / 2.0f, depth - 1);
		rightBottom = new QuadTreeNode(centerX + halfSize / 2.0f, centerZ - halfSize / 2.0f, halfSize / 2.0f, depth - 1);
	}
	else{
		isLeaf = true;
		leftTop = NULL;
		rightTop = NULL;
		leftBottom = NULL;
		rightBottom = NULL;
	}
}

void QuadTreeNode::CalculateBoundingBox()
{
	boundingBox = BoundingBox(Vector3(centerX, 0, centerZ), halfSize, 50.0f, halfSize);
}

void QuadTreeNode::EvaluateVisibility()
{
	isInFrustum = scene.mainCamera.isVisible(boundingBox);

	if(!isInFrustum){
		SetInvisible();
	}
	else{
		if(leftTop)
			leftTop->EvaluateVisibility();
		if(rightTop)
			rightTop->EvaluateVisibility();
		if(leftBottom)
			leftBottom->EvaluateVisibility();
		if(rightBottom)
			rightBottom->EvaluateVisibility();
	}
}

void QuadTreeNode::SetInvisible()
{
	isInFrustum = false;
	if(!isLeaf){
		leftTop->SetInvisible();
		rightTop->SetInvisible();
		leftBottom->SetInvisible();
		rightBottom->SetInvisible();
	}
}