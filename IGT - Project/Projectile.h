#pragma once

#include "iComponents.h"
#include "Collider.h"
#include "ObjectPool.h"

class Projectile 
	: public iUpdateable, public Observer<OverlapEvent, Collider*>
{
public:
	Projectile(GameObject* parent, float damage, float lifeTime)
		:iUpdateable(parent), mDamage(damage), mLifeTime(lifeTime) {}
	~Projectile() = default;

	Projectile* Clone()override;

	void Update(float deltaTime)override;

	void SetParent(GameObject* parent)override;

	void OnNotify(OverlapEvent event, Collider* collider)override;

	void SetObjectPool(ObjectPool<GameObject>* objectPool) { mObjectPool = objectPool; }
private:
	float mDamage;
	float mLifeTime;
	ObjectPool<GameObject>* mObjectPool = nullptr;

	void Destroy();
};