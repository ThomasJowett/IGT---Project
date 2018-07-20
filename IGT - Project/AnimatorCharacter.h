#pragma once
#include "Animator.h"
#include "RigidBody2D.h"

enum CharacterAnimStates
{
	RUN_FORWARD = 0,
	RUN_LEFT,
	RUN_RIGHT,
	RUN_BACK
};

class AnimatorCharacter :
	public Animator<CharacterAnimStates>
{
public:
	AnimatorCharacter(GameObject* parent);
	virtual ~AnimatorCharacter();

	void Update(float deltaTime)override;
	Component* Clone()override;

	virtual void Enter(CharacterAnimStates state);
	virtual void During(CharacterAnimStates state, float deltaTime);
	virtual void Exit(CharacterAnimStates state);

private:
	RigidBody2D * mRigidbody;
};