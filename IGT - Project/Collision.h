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
	std::vector<Contact> DetectCollisions(std::vector<GameObject*>gameObjects);

	std::vector<Contact> DetectCollisions(TileMap* tileMap, std::vector<GameObject*>gameObjects);
	
	void ResolveCollision(Contact contact);
};

#endif // !_COLLISION_H