#include "QuadTree.h"

#include "Texture2D.h"
#include "Sprite.h"
#include "TextRender.h"

QuadTree::QuadTree(AABB* boundary, int level)
	:mBoundary(boundary), mLevel(level)
{
	mNodes[0] = nullptr;
	mNodes[1] = nullptr;
	mNodes[2] = nullptr;
	mNodes[3] = nullptr;

	mColliders = std::vector<Collider*>();
}


QuadTree::~QuadTree()
{
	Clear();
}

void QuadTree::Insert(Collider* collider)
{
	if (mNodes[0] != nullptr)
	{
		int index = GetIndex(collider);

		if (index != -1)
		{
			mNodes[index]->Insert(collider);

			return; //game object successfully inserted to child so exit function
		}
	}

	mColliders.push_back(collider);

	if (mColliders.size() > MAXOBJECTS && mLevel < MAXLEVELS)
	{
		if (mNodes[0] == nullptr)
		{
			Subdivide();
		}

		int i = 0;
		while (i < mColliders.size())
		{
			int index = GetIndex(mColliders.at(i));
			if (index != -1)
			{
				mNodes[index]->Insert(mColliders.at(i));
				mColliders.erase(mColliders.begin() + i);
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
int QuadTree::GetIndex(Collider* collider)
{
	int index = -1;

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

std::vector<Collider*> QuadTree::Retrieve(std::vector<Collider*> &returnObjects, Collider* collider)
{
	int index = GetIndex(collider);

	//If this quadtree is not the leaf node then retrieve from nodes above it
	if (index != -1 && mNodes[0] != nullptr)
	{
		mNodes[index]->Retrieve(returnObjects, collider);
	}

	//Add the objects in this node to the list of return nodes
	for (Collider* thisCollider : mColliders)
	{
		if(thisCollider != collider && (collider->GetParent() != thisCollider->GetParent()))
			returnObjects.push_back(thisCollider);
	}
	return returnObjects;
}

void QuadTree::Clear()
{
	mColliders.clear();

	for (int i = 0; i < 4; i++)
	{
		if (mNodes[i] != nullptr)
		{
			mNodes[i]->Clear();
			delete mNodes[i]->mBoundary;
			delete mNodes[i];
			mNodes[i] = nullptr;
		}
	}

	mIsLeaf = true;
}

void QuadTree::Render(Shader* shader)
{
	Sprite* sprite = new Sprite(nullptr, Texture2D::GetTexture2D("Images/SelectionBox.png"), 
		mBoundary->GetWidth(), mBoundary->GetHeight(), Vector2D(mBoundary->GetPositionX(), mBoundary->GetPositionY()));
	sprite->Render(shader);

	if (!mIsLeaf)
	{
		mNodes[0]->Render(shader);
		mNodes[1]->Render(shader);
		mNodes[2]->Render(shader);
		mNodes[3]->Render(shader);
	}

	delete sprite;
}
