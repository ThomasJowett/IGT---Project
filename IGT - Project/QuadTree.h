#pragma once
#include "GameObject.h"

class AABB
{
public:
	AABB(float width, float height, float positionX, float positionY)
		:mWidth(width), mHeight(height), mPositionX(positionX), mPositionY(positionY) {}
	~AABB();

	float GetWidth() { return mWidth; }
	float GetHeight() { return mHeight; }
	float GetPositionX() { return mPositionX; }
	float GetPositionY() { return mPositionY; }
private:
	float mWidth, mHeight;
	float mPositionX, mPositionY;
};

class QuadTree
{
public:
	QuadTree(AABB* boundary, int level);
	~QuadTree();

	int MAXOBJECTS = 1;
	int MAXLEVELS = 5;

	void Insert(GameObject* gameobject);
	void Subdivide();
	int GetIndex(GameObject* gameobject);
	std::vector<GameObject*>Retrieve(std::vector<GameObject*> &returnObjects, GameObject* gameobject);
private:

	QuadTree * mNodes[4];
	std::vector<GameObject*> mGameObjects;

	AABB* mBoundary;
	int mLevel;
	bool mIsLeaf = true;
};