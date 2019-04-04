#pragma once
#include "iComponents.h"
#include "Messaging.h"
#include "Collider.h"
#include "ObjectPool.h"

enum class AttackEvent {ON_ATTACK_BEGIN, ON_ATTACK_END};
class Attack
	:public iUpdateable, public Subject<AttackEvent, int>
{
public:
	Attack(GameObject* parent, float damage, float cooldown);
	~Attack() = default;

	void Update(float deltaTime) override;

	Attack* Clone()override;

	virtual bool BeginAttack();
	void StopAttack();

	bool GetIsAttacking() const { return mIsAttacking; }
	bool GetIsOnCoolDown() const { return mIsOnCoolDown; }

	void SetParent(GameObject* parent)override;

protected:
	float mDamage;
	float mCoolDown;
	bool mIsAttacking;
	bool mIsOnCoolDown;
	float mCurrentTime;

private:
	Collider* mCollider;
};

class RangedAttack 
	: public Attack
{
public:
	RangedAttack(GameObject* parent, float damage, float cooldown, GameObject* prefab);
	~RangedAttack() = default;

	void Update(float deltaTime)override;

	RangedAttack* Clone()override;

	bool BeginAttack()override;

private:
	ObjectPool<GameObject> mProjectiles;
};