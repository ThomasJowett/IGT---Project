#ifndef _COLLISION_H
#define _COLLISION_H

#include "GameObject.h"
#include <vector>
#include "Collider.h"
#include <memory.h>

struct Contact
{
	GameObject* first;
	GameObject* second;

	Vector2D contactNormal;
	float penetrationDepth;
};

namespace Collision
{
	static std::vector<Contact> DetectCollisions(std::vector<GameObject*>gameObjects)
	{
		std::vector<Contact> contacts;

		for (int i = 0; i < (int)gameObjects.size() - 1; i++)
		{
			for (int j = i + 1; j < (int)gameObjects.size(); j++)
			{
				Collider* collider1 = gameObjects[i]->GetComponent<Collider>();
				Collider* collider2 = gameObjects[j]->GetComponent<Collider>();

				if (collider1 != nullptr && collider2 != nullptr)
				{
					collider1->Notify(OverlapEvent::BEGIN_OVERLAP, gameObjects[j]);
					collider2->Notify(OverlapEvent::BEGIN_OVERLAP, gameObjects[i]);
				
					Vector2D contactNormal;
					float penetrationDepth;
				
					if (collider1->IntersectsCollider(collider2, contactNormal, penetrationDepth))
					{
						contacts.push_back({ gameObjects[i], gameObjects[j], contactNormal, penetrationDepth });
					}

				}
			}
		}
		return contacts;
	}

	static void ResolveCollisions(std::vector<Contact>contacts)
	{
		for (Contact contact : contacts)
		{
			//resolve interpenetration

			//resolve velocities
		}
	}
};

#endif // !_COLLISION_H