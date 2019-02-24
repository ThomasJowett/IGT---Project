#pragma once
#include "GameObject.h"
#include "Collider.h"

class AABB
{
public:
	AABB(float width, float height, float positionX, float positionY)
		:mWidth(width), mHeight(height), mPositionX(positionX), mPositionY(positionY) {}
	~AABB() {}

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

	int MAXOBJECTS = 5;
	int MAXLEVELS = 5;

	void Insert(Collider* collider);
	void Subdivide();
	int GetIndex(Collider* collider);
	std::vector<Collider*>Retrieve(std::vector<Collider*> &returnObjects, Collider* collider);
	void Clear();

	//For Debugging
	void Render(Shader* shader);
private:

	QuadTree * mNodes[4];
	std::vector<Collider*> mColliders;

	AABB* mBoundary;
	int mLevel;
	bool mIsLeaf = true;
};