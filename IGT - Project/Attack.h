#pragma once
#include "iComponents.h"
#include "Messaging.h"
#include "Collider.h"

enum class AttackEvent {ON_ATTACK_BEGIN, ON_ATTACK_END};
class Attack
	:public iUpdateable, public Subject<AttackEvent, int>
{
public:
	Attack(GameObject* parent, float damage, float cooldown);
	~Attack() = default;

	void Update(float deltaTime) override;

	Attack* Clone()override;

	void BeginAttack();
	void StopAttack();

	bool GetIsAttacking() const { return mIsAttacking; }
	bool GetIsOnCoolDown() const { return mIsOnCoolDown; }

	void SetParent(GameObject* parent)override;

private:
	bool mIsAttacking;
	bool mIsOnCoolDown;
	float mDamage;
	float mCurrentTime;
	float mCoolDown;

	Collider* mCollider;
};