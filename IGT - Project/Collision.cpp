#include "Collision.h"

//TODO: make quadtree the size of the tilemap
static QuadTree* quadtree = new QuadTree(new AABB(1600, 1600, 800, -800),0);

std::vector<Contact> Collision::DetectCollisions(std::vector<Collider*> colliders)
{
	quadtree->Clear();
	std::vector<Contact> contacts;
	
	for (Collider* collider : colliders)
	{
		quadtree->Insert(collider);
	}
	
	for (Collider* collider : colliders)
	{
		std::vector<Collider*>coarseList = std::vector<Collider*>();
	
		quadtree->Retrieve(coarseList, collider);
	
		if (coarseList.size())
		{	
			for (Collider* otherCollider : coarseList)
			{	
				if (collider != nullptr && otherCollider != nullptr)
				{
					if (!collider->HasTestedCollisionWith(otherCollider))
					{
						Vector2D contactNormal;
						float penetrationDepth;
	
						if (collider->IntersectsCollider(otherCollider, contactNormal, penetrationDepth))
						{
							collider->Notify(OverlapEvent::BEGIN_OVERLAP, otherCollider);
							otherCollider->Notify(OverlapEvent::BEGIN_OVERLAP, collider);

							if(!collider->IsTrigger() && !otherCollider->IsTrigger())
								contacts.push_back({ collider->GetParent(), otherCollider->GetParent(), contactNormal, penetrationDepth });
						}
	
						collider->AddTestedCollisionWith(otherCollider);
						otherCollider->AddTestedCollisionWith(collider);
					}
				}
			}
		}
		coarseList.clear();
	}
	//for (int i = 0; i < (int)gameObjects.size() - 1; i++)
	//{
	//	Collider* collider1 = gameObjects[i]->GetComponent<Collider>();
	//
	//	for (int j = i + 1; j < (int)gameObjects.size(); j++)
	//	{
	//		Collider* collider2 = gameObjects[j]->GetComponent<Collider>();
	//
	//		if (collider1 != nullptr && collider2 != nullptr)
	//		{
	//			Vector2D contactNormal;
	//			float penetrationDepth;
	//
	//			if (collider1->IntersectsCollider(collider2, contactNormal, penetrationDepth))
	//			{
	//				collider1->Notify(OverlapEvent::BEGIN_OVERLAP, gameObjects[j]);
	//				collider2->Notify(OverlapEvent::BEGIN_OVERLAP, gameObjects[i]);
	//
	//				contacts.push_back({ gameObjects[i], gameObjects[j], contactNormal, penetrationDepth });
	//			}
	//		}
	//	}
	//}
	for (Contact contact : contacts)
	{
		Collision::ResolveCollision(contact);
	}
	return contacts;
}

std::vector<Contact> Collision::DetectCollisions(TileMap * tileMap, std::vector<Collider*>colliders)
{
	std::vector<Contact> contacts;

	if (!tileMap->GetActive())
		return contacts;

	for (int i = 0; i < (int)colliders.size(); i++)
	{
		float Xmax, Xmin, Ymax, Ymin;

		Collider* collider = colliders[i];

		collider->GetBounds(Xmax, Xmin, Ymax, Ymin);

		Ymin -= 1.0f;
		Xmin -= 1.0f;
		
		Ymax += 1.0f;
		Xmax += 1.0f;
		
		Vector2D contactNormal;
		float penetrationDepth;

		std::vector<int> tilesCollided;
		bool alreadyDetected = false;

		//if collider is wider than a tile also test the midpoints
		if (Xmax - Xmin > tileMap->GetTileWidth())
		{
			float midPoint = Xmin + ((Xmax - Xmin) / 2);
			
			
			if (tileMap->GetCollisionAt(Vector2D(midPoint, Ymax)))
			{
				int tile = tileMap->SetColliderPosition(Vector2D(midPoint, Ymax));
				
				for (int tileCollided : tilesCollided)
				{
					if (tileCollided == tile)
					{
						alreadyDetected = true;
						break;
					}
				}

				if (!alreadyDetected)
				{
					tilesCollided.push_back(tile);

					if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
					{
						collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

						if (!collider->IsTrigger())
						{
							contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
						}
					}
				}
			
				alreadyDetected = false;
			}
			
			if (tileMap->GetCollisionAt(Vector2D(midPoint, Ymin)))
			{
				int tile = tileMap->SetColliderPosition(Vector2D(midPoint, Ymin));

				for (int tileCollided : tilesCollided)
				{
					if (tileCollided == tile)
					{
						alreadyDetected = true;
						break;
					}
				}
				if (!alreadyDetected)
				{
					tilesCollided.push_back(tile);
					if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
					{
						collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

						if (!collider->IsTrigger())
						{
							contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
						}
					}
				}

				alreadyDetected = false;
			}
		}

		//if collider is taller than a tile also test the midpoints
		if (Ymax - Ymin > tileMap->GetTileHeight())
		{
			float midPoint = Ymin + ((Ymax - Ymin) / 2);
			
			
			if (tileMap->GetCollisionAt(Vector2D(Xmax, midPoint)))
			{
				int tile = tileMap->SetColliderPosition(Vector2D(Xmax, midPoint));

				for (int tileCollided : tilesCollided)
				{
					if (tileCollided == tile)
					{
						alreadyDetected = true;
						break;
					}
				}
				if (!alreadyDetected)
				{
					tilesCollided.push_back(tile);
					if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
					{
						collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

						if (!collider->IsTrigger())
						{
							contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
						}
					}
				}
				alreadyDetected = false;
			}
			
			if (tileMap->GetCollisionAt(Vector2D(Xmin, midPoint)))
			{
				int tile = tileMap->SetColliderPosition(Vector2D(Xmin, midPoint));

				for (int tileCollided : tilesCollided)
				{
					if (tileCollided == tile)
					{
						alreadyDetected = true;
						break;
					}
				}
				if (!alreadyDetected)
				{
					tilesCollided.push_back(tile);
					if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
					{
						collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

						if (!collider->IsTrigger())
						{
							contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
						}
					}
				}
				alreadyDetected = false;
			}
		}

		//TODO : do a recursive test if the collider is several times taller than the tiles

		//Top Right
		if (tileMap->GetCollisionAt(Vector2D(Xmax, Ymax)))
		{
			int tile = tileMap->SetColliderPosition(Vector2D(Xmax - 1.0f, Ymax - 1.0f));

			for (int tileCollided : tilesCollided)
			{
				if (tileCollided == tile)
				{
					alreadyDetected = true;
					break;
				}
			}
			if (!alreadyDetected)
			{
				tilesCollided.push_back(tile);
				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

					if (!collider->IsTrigger())
					{
						contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
					}

				}
			}
			alreadyDetected = false;
		}
		
		//Top Left
		if (tileMap->GetCollisionAt(Vector2D(Xmin, Ymax)))
		{
			int tile = tileMap->SetColliderPosition(Vector2D(Xmin, Ymax));

			for (int tileCollided : tilesCollided)
			{
				if (tileCollided == tile)
				{
					alreadyDetected = true;
					break;
				}
			}
			if (!alreadyDetected)
			{
				tilesCollided.push_back(tile);

				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

					if (!collider->IsTrigger())
					{
						contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
					}
				}
			}

			alreadyDetected = false;
		}
		
		//Bottom Left
		if (tileMap->GetCollisionAt(Vector2D(Xmin, Ymin)))
		{
			int tile = tileMap->SetColliderPosition(Vector2D(Xmin, Ymin));

			for (int tileCollided : tilesCollided)
			{
				if (tileCollided == tile)
				{
					alreadyDetected = true;
					break;
				}
			}
			if (!alreadyDetected)
			{
				tilesCollided.push_back(tile);

				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

					if (!collider->IsTrigger())
					{
						contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
					}
				}
			}
			alreadyDetected = false;
		}

		//Bottom Right
		if (tileMap->GetCollisionAt(Vector2D(Xmax, Ymin)))
		{
			int tile = tileMap->SetColliderPosition(Vector2D(Xmax, Ymin));

			for (int tileCollided : tilesCollided)
			{
				if (tileCollided == tile)
				{
					alreadyDetected = true;
					break;
				}
			}
			if (!alreadyDetected)
			{
				tilesCollided.push_back(tile);
				if (tileMap->GetCollider()->IntersectsCollider(collider, contactNormal, penetrationDepth))
				{
					collider->Notify(OverlapEvent::BEGIN_OVERLAP, tileMap->GetCollider());

					if (!collider->IsTrigger())
					{
						contacts.push_back({ tileMap, colliders[i]->GetParent(), contactNormal, penetrationDepth });
					}
				}
			}
			alreadyDetected = false;
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

	if(contact.penetrationDepth < 0.0f)
		std::cout << contact.penetrationDepth << std::endl;


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

		float k_slop = 0.000f;

		float sumMasses = bodyA->GetMass() + bodyB->GetMass();

		//resolve interpenetration
		Vector2D translationA = ((contact.normal * contact.penetrationDepth) * ((bodyB->GetMass() / sumMasses)));
		Vector2D translationB = ((contact.normal * -contact.penetrationDepth) * ((bodyA->GetMass() / sumMasses)));
		contact.A->GetLocalTransform()->mPosition += Vector3D(translationA.x, translationA.y, 0);
		contact.B->GetLocalTransform()->mPosition += Vector3D(translationB.x, translationB.y, 0);

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
		Vector2D firstTranslation = contact.normal * max(contact.penetrationDepth, 0.0f);
		contact.A->GetLocalTransform()->mPosition += Vector3D(firstTranslation.x, firstTranslation.y, 0);

		//reflect the velocity
		Vector2D velocity = Vector2D::Reflect(bodyA->GetVelocity(), contact.normal) * bodyA->GetPhysicsMaterial().elasticity;

		if (velocity.SqrMagnitude() < 0.1f)
		{
			velocity = { 0.0f,0.0f };
		}

		bodyA->SetVelocity(velocity);

		return;
	}
	else if (!moveA && moveB) //Object B has no physics so can't move
	{
		//resolve interpenetration
		Vector2D secondTranslation = contact.normal * -(max(contact.penetrationDepth, 0.0f));
		contact.B->GetLocalTransform()->mPosition += Vector3D(secondTranslation.x, secondTranslation.y, 0);

		//reflect the velocity
		Vector2D velocity = Vector2D::Reflect(bodyB->GetVelocity(), contact.normal) * bodyB->GetPhysicsMaterial().elasticity;

		if (velocity.SqrMagnitude() < 0.1f)
		{
			velocity = { 0.0f,0.0f };
		}

		bodyB->SetVelocity(velocity);

		return;
	}
}

QuadTree * Collision::GetQuadtree()
{
	return quadtree;
}
