#pragma once

#include "Animator.h"
#include "RigidBody2D.h"
#include "Attack.h"
#include "Health.h"

enum GoblinAnimstates
{
	GOBLIN_FORWARD = 0,
	GOBLIN_LEFT,
	GOBLIN_RIGHT,
	GOBLIN_BACK,
	GOBLIN_IDLE,
	GOBLIN_ATTACK,
	GOBLIN_DEATH
};

class AnimatorGoblin :
	public Animator<GoblinAnimstates>, public Observer<AttackEvent, int>
{
public:
	AnimatorGoblin(GameObject* parent);
	virtual ~AnimatorGoblin();

	void Update(float deltaTime)override;
	Component* Clone()override;

	virtual void Enter(GoblinAnimstates state);
	virtual void During(GoblinAnimstates state, float deltaTime);
	virtual void Exit(GoblinAnimstates state);

	void CreateAnimations()override;

	void OnNotify(AnimationNotify notify, int chanel) override;
	void OnNotify(AttackEvent notify, int payload)override;

	void SetParent(GameObject* parent)override;

private:
	RigidBody2D * mRigidbody;
	Attack* mAttack;
	bool mIsAttacking;
};