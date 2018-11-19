#include "Astar.h"

PathFinding::PathFinding(void)
{
	mInitialzedStartGoal = false;
	mFoundGoal = false;
}

PathFinding::~PathFinding(void)
{
}

void PathFinding::FindPath(Vector2D currentPos, Vector2D targetPos)
{
	if (!mInitialzedStartGoal)
	{
		for (int i = 0; i < mOpenList.size(); i++)
		{
			delete mOpenList[1];
		}
		mOpenList.clear();

		for (int i = 0; i < mClosedList.size(); i++)
		{
			delete mClosedList[i];
		}
		mClosedList.clear();

		for (int i = 0; mPathToGoal.size(); i++)
		{
			delete mPathToGoal[i];
		}
		mPathToGoal.clear();

		SearchCell start;
		start.mX = currentPos.x;
		start.mY = currentPos.y;

		//initialze goal
		SearchCell goal;
		goal.mX = targetPos.x;
		goal.mY = targetPos.y;

		SetStartAndGoal(start, goal);
		mInitialzedStartGoal = true;
	}

	if (mInitialzedStartGoal)
	{
		ContinuePath();
	}
}

Vector2D PathFinding::NextPathPos(GameObject* gameObject)
{
	int index = 1;

	Vector2D nextPos;
	nextPos.x = mPathToGoal[mPathToGoal.size() - index]->x;
	nextPos.y = mPathToGoal[mPathToGoal.size() - index]->y;

	Vector2D distance = nextPos - Vector2D(gameObject->GetWorldTransform()->mPosition.x, gameObject->GetWorldTransform()->mPosition.y);

	if (index < mPathToGoal.size())
	{
		//if distance to next position is small enough
		if (distance.Magnitude() < 1.0f)
		{
			mPathToGoal.erase(mPathToGoal.end() - index);
		}
	}

	return nextPos;
}

void PathFinding::SetStartAndGoal(SearchCell start, SearchCell goal)
{
	mStartCell = new SearchCell(start.mX, start.mY, 0);
	mGoalCell = new SearchCell(goal.mX, goal.mY, &goal);

	mStartCell->G = 0;
	mStartCell->H = mStartCell->ManhattanDistance(mGoalCell);
	mStartCell->mParent = 0;

	mOpenList.push_back(mStartCell);
}

void PathFinding::PathOpened(int x, int y, float newCost, SearchCell * parent)
{
	//if(tilemap->getCollisionAt(x,y))
	//{
	//	return;
	//}

	int id = y * WORLD_SIZE + x;

	for (int i = 0; i < mClosedList.size(); i++)
	{
		if (id == mClosedList[i]->mID)
		{
			return;
		}
	}

	SearchCell * newChild = new SearchCell(x, y, parent);
	newChild->G = newCost;
	newChild->H = parent->ManhattanDistance(mGoalCell);

	for (int i = 0; i < mOpenList.size(); i++)
	{
		if (id == mOpenList[i]->mID)
		{
			float newF = newChild->G + newCost + mOpenList[i]->H;

			if (mOpenList[i]->GetF() > newF)
			{
				mOpenList[i]->G = newChild->G + newCost;
				mOpenList[i]->mParent = newChild;
			}
			else
			{
				delete newChild;
				return;
			}
		}
	}

	mOpenList.push_back(newChild);
}

SearchCell * PathFinding::GetNextCell()
{
	float bestF = FLT_MAX;
	int cellIndex = -1;
	SearchCell* nextCell = NULL;

	for (int i = 0; i < mOpenList.size(); i++)
	{
		if (mOpenList[i]->GetF() < bestF)
		{
			bestF = mOpenList[i]->GetF();
			cellIndex = i;
		}
	}

	if (cellIndex > 0)
	{
		nextCell = mOpenList[cellIndex];
		mClosedList.push_back(nextCell);
		mOpenList.erase(mOpenList.begin() + cellIndex);
	}

	return nextCell;
}

void PathFinding::ContinuePath()
{
	if (mOpenList.empty())
	{
		return;
	}

	SearchCell* currentCell = GetNextCell();

	if (currentCell->mID == mGoalCell->mID)
	{
		mGoalCell->mParent = currentCell->mParent;

		SearchCell* getPath;

		for (getPath = mGoalCell; getPath != NULL; getPath = getPath->mParent)
		{
			mPathToGoal.push_back(new Vector2D(getPath->mX, getPath->mY));
		}

		mFoundGoal = true;
		return;
	}
	else
	{
		//Right
		PathOpened(currentCell->mX +1, currentCell->mY, currentCell->G +1.0f, currentCell);

		//Left
		PathOpened(currentCell->mX - 1, currentCell->mY, currentCell->G + 1.0f, currentCell);

		//Top
		PathOpened(currentCell->mX, currentCell->mY + 1, currentCell->G + 1.0f, currentCell);

		//Bottom
		PathOpened(currentCell->mX, currentCell->mY - 1, currentCell->G + 1.0f, currentCell);

		//diagonals cost is square root 2
		//Top Right
		PathOpened(currentCell->mX + 1, currentCell->mY + 1, currentCell->G + 1.414f, currentCell);

		//Top Left
		PathOpened(currentCell->mX - 1, currentCell->mY + 1, currentCell->G + 1.414f, currentCell);

		//Bottom Right
		PathOpened(currentCell->mX + 1, currentCell->mY - 1, currentCell->G + 1.414f, currentCell);

		//Bottom Left
		PathOpened(currentCell->mX - 1, currentCell->mY - 1, currentCell->G + 1.414f, currentCell);

		for (int i = 0; i < mOpenList.size(); i++)
		{
			if (currentCell->mID == mOpenList[i]->mID)
			{
				mOpenList.erase(mOpenList.begin() + i);
			}
		}
	}
}
