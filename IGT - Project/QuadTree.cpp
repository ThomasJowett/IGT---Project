#include "QuadTree.h"
#include "Collider.h"


QuadTree::QuadTree(AABB* boundary, int level)
	:mBoundary(boundary), mLevel(level)
{
	mNodes[0] = nullptr;
	mNodes[1] = nullptr;
	mNodes[2] = nullptr;
	mNodes[3] = nullptr;

	mGameObjects = std::vector<GameObject*>();

}


QuadTree::~QuadTree()
{
}

void QuadTree::Insert(GameObject * gameobject)
{
	if (mNodes[0] != nullptr)
	{
		int index = GetIndex(gameobject);

		if (index != -1)
		{
			mNodes[index]->Insert(gameobject);

			return; //game object successfully inserted to child so exit function
		}
	}

	mGameObjects.push_back(gameobject);

	if (mGameObjects.size() > MAXOBJECTS && mLevel < MAXLEVELS)
	{
		if (mNodes[0] == nullptr)
		{
			Subdivide();
		}

		int i = 0;
		while (i < mGameObjects.size())
		{
			int index = GetIndex(mGameObjects.at(i));
			if (index != -1)
			{
				mNodes[index]->Insert(mGameObjects.at(i));
				mGameObjects.erase(mGameObjects.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}
}

//Adds four quadtree nodes as children
void QuadTree::Subdivide()
{
	mIsLeaf = false;

	float halfWidth = mBoundary->GetWidth() / 2;
	float halfHeight = mBoundary->GetHeight() / 2;

	float height = mBoundary->GetHeight();

	mNodes[0] = new QuadTree(new AABB(halfWidth, halfHeight, mBoundary->GetPositionX() - halfWidth/2, mBoundary->GetPositionY() + halfHeight/2), mLevel + 1);
	mNodes[1] = new QuadTree(new AABB(halfWidth, halfHeight, mBoundary->GetPositionX() + halfWidth/2, mBoundary->GetPositionY() + halfHeight/2), mLevel + 1);
	mNodes[2] = new QuadTree(new AABB(halfWidth, halfHeight, mBoundary->GetPositionX() - halfWidth/2, mBoundary->GetPositionY() - halfHeight/2), mLevel + 1);
	mNodes[3] = new QuadTree(new AABB(halfWidth, halfHeight, mBoundary->GetPositionX() + halfWidth/2, mBoundary->GetPositionY() - halfHeight/2), mLevel + 1);
}

//returns which node the gameobject should be inserted into
int QuadTree::GetIndex(GameObject * gameobject)
{
	int index = -1;
	Collider* collider = gameobject->GetComponent<Collider>();
	bool topHalf = (collider->TestAxis(Vector2D(0, 1), mBoundary->GetPositionY())
		&& collider->TestAxis(Vector2D(0, -1), (mBoundary->GetHeight()/2) + mBoundary->GetPositionY()));
	bool bottomHalf = (collider->TestAxis(Vector2D(0, -1), mBoundary->GetPositionY())
		&& collider->TestAxis(Vector2D(0, -1), mBoundary->GetPositionY() - (mBoundary->GetHeight()/2)));

	float boundary = mBoundary->GetHeight();

	if (collider->TestAxis(Vector2D(1,0), mBoundary->GetPositionX())
		&& collider->TestAxis(Vector2D(-1, 0), mBoundary->GetPositionX() + (mBoundary->GetWidth()/2)))
	{
		if (topHalf)
			index = 1;
		else if (bottomHalf)
			index = 3;
	}
	else if (collider->TestAxis(Vector2D(-1, 0), mBoundary->GetPositionX())
		&& collider->TestAxis(Vector2D(1, 0), mBoundary->GetPositionX() - (mBoundary->GetWidth()/2)))
	{
		if (topHalf)
			index = 0;
		else if (bottomHalf)
			index = 2;
	}

	return index;
}

std::vector<GameObject*> QuadTree::Retrieve(std::vector<GameObject*>& returnObjects, GameObject * gameobject)
{
	int index = GetIndex(gameobject);

	//If this quadtree is not the leaf node then retrieve from nodes above it
	if (index != -1 && mNodes[0] != nullptr)
	{
		mNodes[index]->Retrieve(returnObjects, gameobject);
	}

	//Add the objects in this node to the list of return nodes
	for (GameObject* gameObject : mGameObjects)
	{
		if(gameObject != gameobject)
			returnObjects.push_back(gameObject);
	}
	return returnObjects;
}