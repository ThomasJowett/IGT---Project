#ifndef _COLLISION_H
#define _COLLISION_H

#include "GameObject.h"
#include <vector>
#include "Collider.h"
#include "RigidBody2D.h"
#include "QuadTree.h"
#include "TileMap.h"
#include <memory.h>

struct Contact
{
	GameObject* A;
	GameObject* B;

	Vector2D normal;
	float penetrationDepth;
};

namespace Collision
{
	std::vector<Contact> DetectCollisions(std::vector<Collider*> colliders);

	std::vector<Contact> DetectCollisions(TileMap * tileMap, std::vector<Collider*>colliders);
	
	void ResolveCollision(Contact contact);

	QuadTree* GetQuadtree();
};

#endif // !_COLLISION_H