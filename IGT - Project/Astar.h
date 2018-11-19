#pragma once

#include <math.h>
#include <vector>
#include "Vector.h"
#include "GameObject.h"

#define WORLD_SIZE 64

struct SearchCell
{
public:
	int mX, mY;
	int mID;
	SearchCell * mParent;
	float G;
	float H;

	SearchCell() : mParent(0) {}
	SearchCell(int x, int y, SearchCell * parent = 0)
		: mX(x), mY(y), mParent(parent), mID(y * WORLD_SIZE + y), G(0), H(0) {};

	float GetF() { return G + H; }
	float ManhattanDistance(SearchCell * nodeEnd)
	{
		float x = (float)(fabs(this->mX - nodeEnd->mX));
		float y = (float)(fabs(this->mY - nodeEnd->mY));

		return x + y;
	}
};

class PathFinding
{
public:
	PathFinding(void);
	~PathFinding(void);

	void FindPath(Vector2D currentPos, Vector2D targetPos);
	Vector2D NextPathPos(GameObject* gameObject);
	void ClearOpenList() { mOpenList.clear(); }
	void ClearClosedList() { mClosedList.clear(); }
	void ClearPathToGoal() { mPathToGoal.clear(); }

	bool mInitialzedStartGoal;
	bool mFoundGoal;

private:
	void SetStartAndGoal(SearchCell start, SearchCell goal);
	void PathOpened(int x, int y, float newCost, SearchCell * parent);
	SearchCell * GetNextCell();
	void ContinuePath();

	SearchCell* mStartCell;
	SearchCell * mGoalCell;

	std::vector<SearchCell*> mOpenList;
	std::vector<SearchCell*> mClosedList;
	std::vector<Vector2D*> mPathToGoal;
};