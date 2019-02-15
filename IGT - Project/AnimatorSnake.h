#pragma once
#include "Animator.h"
#include "RigidBody2D.h"
#include "Attack.h"
#include "Health.h"

enum SnakeAnimstates
{
	SNAKE_IDLE = 0,
	SNAKE_TURN_LEFT,
	SNAKE_TURN_RIGHT,
	SNAKE_LEFT,
	SNAKE_RIGHT,
	SNAKE_ATTACK,
	SNAKE_DEATH
};
class AnimatorSnake :
	public Animator<SnakeAnimstates>, public Observer<AttackEvent, int>, public Observer<HealthEvent, GameObject*>
{
public:
	AnimatorSnake(GameObject* parent);
	virtual ~AnimatorSnake();

	void Update(float deltaTime)override;
	Component* Clone()override;

	void Enter(SnakeAnimstates state)override;
	void During(SnakeAnimstates state, float deltaTime)override;
	void Exit(SnakeAnimstates state)override;

	void CreateAnimations()override;

	void OnNotify(AnimationNotify notify, int chanel) override;
	void OnNotify(AttackEvent notify, int payload)override;
	void OnNotify(HealthEvent notify, GameObject* gameObject)override;

private:
	RigidBody2D * mRigidbody;
	Attack * mAttack;
	bool mIsAttacking;
};