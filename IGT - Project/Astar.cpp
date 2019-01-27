#include "Astar.h"

static Astar::Generator* instance = 0;

Astar::Node::Node(int2 coordinates, Node * parent)
	:mParent(parent), mCoordinates(coordinates)
{
	G = H = 0;
}

unsigned int Astar::Node::GetScore()
{
	return G + H;
}

Astar::Generator::Generator()
{
	SetDiagonalMovement(true);
	SetHeuristic(&Heuristic::Manhattan);

	mDirection = {
		{ 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
		{ -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
	};
}

bool Astar::Generator::DetectCollision(int2 coordinates)
{
	if (std::find(mCollisions.begin(), mCollisions.end(), coordinates) == mCollisions.end())
	{
		return (mTileMap->GetCollisionAt(coordinates.x, coordinates.y));
	}
	else
		return true;
}

Astar::Node * Astar::Generator::FindNodeOnList(NodeSet & nodes, int2 coordinates)
{
	for (Node* node : nodes)
	{
		if (node->mCoordinates == coordinates)
		{
			return node;
		}
	}
	return nullptr;
}

void Astar::Generator::ReleaseNodes(NodeSet & nodes)
{
	for (auto it = nodes.begin(); it != nodes.end();)
	{
		delete *it;
		it = nodes.erase(it);
	}
}

Astar::Generator * Astar::Generator::GetInstance()
{
	if (instance == 0)
	{
		instance = new Generator();
	}
	return instance;
}

void Astar::Generator::SetTileMap(TileMap * tilemap)
{
	mTileMap = tilemap;
}

void Astar::Generator::SetDiagonalMovement(bool enable)
{
	mDirections = (enable ? 8 : 4);
}

void Astar::Generator::SetHeuristic(HeuristicFunction function)
{
	mHeuristic = std::bind(function, std::placeholders::_1, std::placeholders::_2);
}

std::vector<Vector2D> Astar::Generator::FindPath(Vector2D source, Vector2D target)
{
	std::vector<Vector2D> path;

	if (!mTileMap->GetActive())
		return path;

	/*Convert the positions to coordinates on the grid*/
	unsigned int x,y;

	if (!mTileMap->PositionToTileIndex(source, x, y))
		return path;//if source is not on tile map then return no path
	int2 sourceCoords = { x,y };

	if (!mTileMap->PositionToTileIndex(target, x, y))
		return path;//if target is not on tile map then return no path
	int2 targetCoords = { x,y };

	Node* current = nullptr;
	NodeSet openSet, closedSet;
	openSet.insert(new Node(sourceCoords));

	while (!openSet.empty())
	{
		current = *openSet.begin();
		for (Node* node : openSet)
		{
			if (node->GetScore() <= current->GetScore())
			{
				current = node;
			}
		}

		if (current->mCoordinates == targetCoords)
		{
			break;//found the target so exit
		}

		closedSet.insert(current);
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));

		for (unsigned int i = 0; i < mDirections; ++i)
		{
			int2 newCoordinates(current->mCoordinates + mDirection[i]);

			if (DetectCollision(newCoordinates) || FindNodeOnList(closedSet, newCoordinates))
			{
				continue;
			}

			unsigned int totalcost = current->G + ((i < 4) ? 10 : 14);

			Node * successor = FindNodeOnList(openSet, newCoordinates);
			if (successor == nullptr)
			{
				successor = new Node(newCoordinates, current);
				successor->G = totalcost;
				successor->H = mHeuristic(successor->mCoordinates, targetCoords);
				openSet.insert(successor);
			}
			else if (totalcost < successor->G)
			{
				successor->mParent = current;
				successor->G = totalcost;
			}
		}
	}

	
	while (current != nullptr)
	{
		/*convert coordinates back to world positions*/
		Vector2D position;
		mTileMap->TileIndexToPosition(current->mCoordinates.x, current->mCoordinates.y, position);
		path.push_back(position);
		current = current->mParent;
	}

	ReleaseNodes(openSet);
	ReleaseNodes(closedSet);
	return path;
}

void Astar::Generator::AddCollision(Vector2D coordinates)
{
	unsigned int x, y;
	mTileMap->PositionToTileIndex(coordinates, x, y);
	mCollisions.push_back({ (int)x, (int)y});
}

void Astar::Generator::RemoveCollision(Vector2D coordinates)
{
	unsigned int x, y;
	mTileMap->PositionToTileIndex(coordinates, x, y);

	int2 coords = { x,y };
	std::vector<int2>::iterator it = std::find(mCollisions.begin(), mCollisions.end(), coords);
	if (it != mCollisions.end())
	{
		mCollisions.erase(it);
	}
}

void Astar::Generator::ClearCollisions()
{
	mCollisions.clear();
}

int2 Astar::Heuristic::GetDelta(int2 source, int2 target)
{
	return { abs(source.x - target.x), abs(source.y - target.y) };
}

unsigned int Astar::Heuristic::Manhattan(int2 source, int2 target)
{
	auto delta = std::move(GetDelta(source, target));
	return static_cast<unsigned int>(10 * (delta.x + delta.y));
}

unsigned int Astar::Heuristic::Euclidean(int2 source, int2 target)
{
	auto delta = std::move(GetDelta(source, target));
	return static_cast<unsigned int>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

unsigned int Astar::Heuristic::Octagonal(int2 source, int2 target)
{
	auto delta = std::move(GetDelta(source, target));
	return 10 * (delta.x + delta.y) + (-6) * min(delta.x, delta.y);
}
