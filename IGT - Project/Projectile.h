#pragma once

#include "iComponents.h"
#include "Collider.h"
#include "RigidBody2D.h"
#include "ObjectPool.h"
#include <iostream>

class Projectile 
	: public iUpdateable, public Observer<OverlapEvent, Collider*>
{
public:
	Projectile(GameObject* parent, float damage, float lifeTime);
	~Projectile() = default;

	Projectile* Clone()override;

	void Update(float deltaTime)override;

	void SetParent(GameObject* parent)override;

	void OnNotify(OverlapEvent event, Collider* collider)override;

	void Reset(ObjectPool<GameObject>* objectPool, GameObject* instigator);
private:
	float mDamage;
	float mLifeTime;
	float mInitialLifeTime;
	ObjectPool<GameObject>* mObjectPool = nullptr;

	GameObject* mInstigator;

	RigidBody2D* mRigidBody;

	void Destroy();
};