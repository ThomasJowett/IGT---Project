#include "QuadTree.h"
#include "Collider.h"
#include "Texture2D.h"

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
	Clear();
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

	float Xmax, Xmin, Ymax, Ymin;

	collider->GetBounds(Xmax, Xmin, Ymax, Ymin);

	bool topHalf = (Ymin > mBoundary->GetPositionY() && Ymax < (mBoundary->GetHeight()/2 + mBoundary->GetPositionY()));

	bool bottomHalf = (Ymax < mBoundary->GetPositionY() && Ymin > (mBoundary->GetPositionY() - (mBoundary->GetHeight()/2)));

	float boundary = mBoundary->GetHeight();

	//right half
	if (Xmin > mBoundary->GetPositionX() && Xmax < (mBoundary->GetPositionX() + (mBoundary->GetWidth()/2)))
	{
		if (topHalf)
			index = 1;
		else if (bottomHalf)
			index = 3;
	}
	//left half
	else if (Xmax < mBoundary->GetPositionX() && Xmin > (mBoundary->GetPositionX() - (mBoundary->GetWidth()/2)))
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

void QuadTree::Clear()
{
	mGameObjects.clear();

	for (int i = 0; i < 4; i++)
	{
		if (mNodes[i] != nullptr)
		{
			mNodes[i]->Clear();
			mNodes[i] = nullptr;
		}
	}
}