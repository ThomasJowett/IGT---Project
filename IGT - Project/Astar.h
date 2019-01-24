#pragma once

#include <math.h>
#include <vector>
#include <functional>
#include <set>
#include "Vector.h"
#include "GameObject.h"
#include "TileMap.h"

namespace Astar
{
	using HeuristicFunction = std::function<unsigned int(int2, int2)>;

struct Node
{
	unsigned int G, H;
	int2 mCoordinates;
	Node * mParent;

	Node(int2 coordinates, Node * parent = nullptr);

	unsigned int GetScore();
};

using NodeSet = std::set<Node*>;

class Generator
{
	Generator();
	bool DetectCollision(int2 coordinates);
	Node* FindNodeOnList(NodeSet& nodes, int2 coordinates);
	void ReleaseNodes(NodeSet& nodes);

public:
	static Generator* GetInstance();

	void SetTileMap(TileMap* tilemap);
	void SetDiagonalMovement(bool enable);
	void SetHeuristic(HeuristicFunction function);
	std::vector<Vector2D> FindPath(Vector2D source, Vector2D target);
	void AddCollision(Vector2D coordinates);
	void RemoveCollision(Vector2D coordinates);
	void ClearCollisions();

private:
	HeuristicFunction mHeuristic;
	std::vector<int2> mDirection, mCollisions;
	TileMap* mTileMap = nullptr;
	unsigned int mDirections;
};

class Heuristic
{
	static int2 GetDelta(int2 source, int2 target);

public:
	static unsigned int Manhattan(int2 source, int2 target);
	static unsigned int Euclidean(int2 source, int2 target);
	static unsigned int Octagonal(int2 source, int2 target);
};

}