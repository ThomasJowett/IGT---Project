#include "AnimatorCharacter.h"



AnimatorCharacter::AnimatorCharacter(GameObject* parent)
	:Animator(parent)
{
	mRigidbody = GetParent()->GetComponent<RigidBody2D>();

	//Create all the animations
	
	mAnimations.emplace_back(std::make_unique<Animation>(0, 3, 0.1, mSprite));//Forward
	mAnimations.emplace_back(std::make_unique<Animation>(3, 3, 0.1, mSprite));//Left
	mAnimations.emplace_back(std::make_unique<Animation>(6, 3, 0.1, mSprite));//Right
	mAnimations.emplace_back(std::make_unique<Animation>(9, 3, 0.1, mSprite));//Back
}


AnimatorCharacter::~AnimatorCharacter()
{
}

void AnimatorCharacter::Update(float deltaTime)
{
	Vector2D velocity = mRigidbody->GetVelocity();

	if (velocity.x > 0.1f)
		ChangeState(RUN_RIGHT);
	else if (velocity.x < -0.1f)
		ChangeState(RUN_LEFT);
	else if (velocity.y < -0.1f)
		ChangeState(RUN_FORWARD);
	else if (velocity.y > 0.1f)
		ChangeState(RUN_BACK);

	Animator::Update(deltaTime);
}

Component * AnimatorCharacter::Clone()
{
	return new AnimatorCharacter(nullptr);
}

void AnimatorCharacter::Enter(CharacterAnimStates state)
{
	switch (state)
	{
	case RUN_FORWARD:
		break;
	case RUN_RIGHT:
		break;
	case RUN_LEFT:
		break;
	case RUN_BACK:
		break;
	default:
		break;
	}
}

void AnimatorCharacter::During(CharacterAnimStates state)
{
	switch (state)
	{
	case RUN_FORWARD:
		break;
	case RUN_RIGHT:
		break;
	case RUN_LEFT:
		break;
	case RUN_BACK:
		break;
	default:
		break;
	}
}

void AnimatorCharacter::Exit(CharacterAnimStates state)
{
	switch (state)
	{
	case RUN_FORWARD:
		break;
	case RUN_RIGHT:
		break;
	case RUN_LEFT:
		break;
	case RUN_BACK:
		break;
	default:
		break;
	}
}
