#include "Collision.h"

std::vector<Contact> Collision::DetectCollisions(std::vector<GameObject*> gameObjects)
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
				Vector2D contactNormal;
				float penetrationDepth;
	
				if (collider1->IntersectsCollider(collider2, contactNormal, penetrationDepth))
				{
					collider1->Notify(OverlapEvent::BEGIN_OVERLAP, gameObjects[j]);
					collider2->Notify(OverlapEvent::BEGIN_OVERLAP, gameObjects[i]);
					
					contacts.push_back({ gameObjects[i], gameObjects[j], contactNormal, penetrationDepth });
				}
			}
		}
	}
	for (Contact contact : contacts)
	{
		Collision::ResolveCollision(contact);
	}

	return contacts;
}

std::vector<Contact> Collision::DetectCollisions(TileMap * tileMap, std::vector<GameObject*>gameObjects)
{
	std::vector<Contact> contacts;

	for (int i = 0; i < (int)gameObjects.size(); i++)
	{
		float Xmax, Xmin, Ymax, Ymin;

		Collider* collider = gameObjects[i]->GetComponent<Collider>();

		collider->GetBounds(Xmax, Xmin, Ymax, Ymin);

		Ymin -= 1.0f;
		Xmin -= 1.0f;

		Ymax += 1.0f;
		Xmax += 1.0f;
		
		Vector2D contactNormal;
		float penetrationDepth;

		//if collider is wider than a tile also test the midpoints
		if (Xmax - Xmin > tileMap->GetTileWidth())
		{
			float midPoint = Xmin + ((Xmax - Xmin) / 2);
			
			
			if (tileMap->GetCollisionAt(Vector2D(midPoint, Ymax)))
			{
				tileMap->SetColliderPosition(Vector2D(midPoint, Ymax));
			
				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
				}
			}
			
			if (tileMap->GetCollisionAt(Vector2D(midPoint, Ymin)))
			{
				tileMap->SetColliderPosition(Vector2D(midPoint, Ymin));
			
				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
				}
			}
		}

		//if collider is taller than a tile also test the midpoints
		if (Ymax - Ymin > tileMap->GetTileHeight())
		{
			float midPoint = Ymin + ((Ymax - Ymin) / 2);
			
			
			if (tileMap->GetCollisionAt(Vector2D(Xmax, midPoint)))
			{
				tileMap->SetColliderPosition(Vector2D(Xmax, midPoint));
			
				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
				}
			}
			
			if (tileMap->GetCollisionAt(Vector2D(Xmin, midPoint)))
			{
				tileMap->SetColliderPosition(Vector2D(Xmin, midPoint));
			
				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
				}
			}
		}

		//TODO : do a recursive test if the collider is several times taller than the tiles

		//Top Right
		if (tileMap->GetCollisionAt(Vector2D(Xmax, Ymax)))
		{
			tileMap->SetColliderPosition(Vector2D(Xmax - 1.0f, Ymax - 1.0f));
		
			if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
			{
				contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
			}
		}
		
		//Top Left
		if (tileMap->GetCollisionAt(Vector2D(Xmin, Ymax)))
		{
			tileMap->SetColliderPosition(Vector2D(Xmin, Ymax));
		
			if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
			{
				contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
			}
		}
		
		//Bottom Left
		if (tileMap->GetCollisionAt(Vector2D(Xmin, Ymin)))
		{
			tileMap->SetColliderPosition(Vector2D(Xmin, Ymin));
		
			if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
			{
				contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
			}
		}

		//Bottom Right
		if (tileMap->GetCollisionAt(Vector2D(Xmax, Ymin)))
		{
			tileMap->SetColliderPosition(Vector2D(Xmax, Ymin));
		
			if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
			{
				contacts.push_back({ tileMap, gameObjects[i], contactNormal, penetrationDepth });
			}
		}

		for (Contact contact : contacts)
		{
			Collision::ResolveCollision(contact);
		}
		contacts.clear();
	}
	return contacts;
}

void Collision::ResolveCollision(Contact contact)
{
	//Cache bodies
	RigidBody2D *bodyA, *bodyB;

	bodyA = contact.A->GetComponent<RigidBody2D>();
	bodyB = contact.B->GetComponent<RigidBody2D>();


	bool moveA = (bodyA != nullptr);
	bool moveB = (bodyB != nullptr);


	if (!moveA && !moveB)
	{
		//both objects can not be moved so do nothing
		return;
	}
	else if (moveA && moveB)
	{
		//cache variables
		Vector2D velocityA = bodyA->GetVelocity();
		Vector2D velocityB = bodyB->GetVelocity();

		float inverseMassA = bodyA->GetInverseMass();
		float inverseMassB = bodyB->GetInverseMass();

		float coeffiecientOfRestitution = (bodyA->GetPhysicsMaterial().elasticity + bodyB->GetPhysicsMaterial().elasticity) / 2;

		//resolve interpenetration
		Vector2D firstTranslation = ((contact.normal * contact.penetrationDepth) * ((inverseMassB / inverseMassA + inverseMassB)));
		Vector2D secondTranslation = ((contact.normal * -contact.penetrationDepth) * ((inverseMassA / inverseMassA + inverseMassB)));
		contact.A->GetTransform()->mPosition += Vector3D(firstTranslation.x, firstTranslation.y, 0);
		contact.B->GetTransform()->mPosition += Vector3D(secondTranslation.x, secondTranslation.y, 0);

		//relative velocity
		Vector2D relativeVelocity = velocityB - velocityA;

		//relative velocity along contact normal
		float contactVelocity = Vector2D::Dot(relativeVelocity, contact.normal);

		//Calculate impulse
		float j = -(1 + coeffiecientOfRestitution) * contactVelocity;
		j /= inverseMassA + inverseMassB;
		Vector2D impulse = contact.normal * j;

		//Apply impulse
		bodyA->ApplyImpulse(-impulse);
		bodyB->ApplyImpulse(impulse);

		return;
	}
	else if (moveA && !moveB)//Object A has no physics so can't move
	{
		//resolve interpenetration
		Vector2D firstTranslation = contact.normal * contact.penetrationDepth;
		contact.A->GetTransform()->mPosition += Vector3D(firstTranslation.x, firstTranslation.y, 0);

		//reflect the velocity
		bodyA->SetVelocity(Vector2D::Reflect(bodyA->GetVelocity(), contact.normal) * bodyA->GetPhysicsMaterial().elasticity);

		return;
	}
	else if (!moveA && moveB) //Object B has no physics so can't move
	{
		//resolve interpenetration
		Vector2D secondTranslation = contact.normal * -contact.penetrationDepth;
		contact.B->GetTransform()->mPosition += Vector3D(secondTranslation.x, secondTranslation.y, 0);

		//reflect the velocity
		bodyB->SetVelocity(Vector2D::Reflect(bodyB->GetVelocity(), contact.normal) * bodyB->GetPhysicsMaterial().elasticity);

		return;
	}
}
