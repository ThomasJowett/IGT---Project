#include "Collision.h"

std::vector<Contact> Collision::DetectCollisions(std::vector<GameObject*> gameObjects)
{
	QuadTree* quadTree = new QuadTree(new AABB(1000, 1000, 0, 0), 0);
	
	for (auto gameObject : gameObjects)
	{
		if (gameObject->GetComponent<Collider>())
			quadTree->Insert(gameObject);
	}

	std::vector<Contact> contacts;

	for (GameObject* gameobject : gameObjects)
	{
		std::vector<GameObject*>coarseList = std::vector<GameObject*>();
		quadTree->Retrieve(coarseList, gameobject);
	
		if (coarseList.size())
		{
			for (GameObject* otherObject : coarseList)
			{
				Collider* collider1 = gameobject->GetComponent<Collider>();
				Collider* collider2 = otherObject->GetComponent<Collider>();
	
				Vector2D contactNormal;
				float penetrationDepth;
	
				if (collider1->IntersectsCollider(collider2, contactNormal, penetrationDepth))
				{
					contacts.push_back({ gameobject, otherObject, contactNormal, penetrationDepth });
				}
			}
		}
		Collision::ResolveCollisions(contacts);
		contacts.clear();
	}



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
	Collision::ResolveCollisions(contacts);

	delete quadTree;
	return contacts;
}

void Collision::ResolveCollisions(std::vector<Contact> contacts)
{
	{
		for (Contact contact : contacts)
		{
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
				contact.A->GetComponent<RigidBody2D>()->ApplyImpulse(-impulse);
				contact.B->GetComponent<RigidBody2D>()->ApplyImpulse(impulse);
			}
			else if (moveA && !moveB)//Object A has no physics so can't move
			{
				//resolve interpenetration
				Vector2D firstTranslation = contact.normal * contact.penetrationDepth;
				contact.A->GetTransform()->mPosition += Vector3D(firstTranslation.x, firstTranslation.y, 0);

				//reflect the velocity
				bodyA->SetVelocity(Vector2D::Reflect(bodyA->GetVelocity(), contact.normal) * bodyA->GetPhysicsMaterial().elasticity);
			}
			else if (!moveA && moveB) //Object B has no physics so can't move
			{
				//resolve interpenetration
				Vector2D secondTranslation = contact.normal * -contact.penetrationDepth;
				contact.B->GetTransform()->mPosition += Vector3D(secondTranslation.x, secondTranslation.y, 0);

				//reflect the velocity
				bodyB->SetVelocity(Vector2D::Reflect(bodyB->GetVelocity(), contact.normal) * bodyB->GetPhysicsMaterial().elasticity);
			}
		}
	}
}
