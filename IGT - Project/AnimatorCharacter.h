#pragma once
#include "Animator.h"
#include "RigidBody2D.h"

enum CharacterAnimStates
{
	CHARACTER_FORWARD = 0,
	CHARACTER_LEFT,
	CHARACTER_RIGHT,
	CHARACTER_BACK,
	CHARACTER_IDLE,
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

	void CreateAnimations()override;
private:
	RigidBody2D * mRigidbody;
};