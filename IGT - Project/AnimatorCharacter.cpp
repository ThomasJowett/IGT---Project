#include "AnimatorCharacter.h"



AnimatorCharacter::AnimatorCharacter(GameObject* parent)
	:Animator(parent)
{
	if (GetParent())
	{
		mRigidbody = GetParent()->GetComponent<RigidBody2D>();
		mAttack = GetParent()->GetComponent<Attack>();
		mAttack->AddObserver(this);

		CreateAnimations();
	}
}


AnimatorCharacter::~AnimatorCharacter()
{
}

void AnimatorCharacter::Update(float deltaTime)
{
	if (!mIsAttacking)
	{
		if (!mRigidbody)
			mRigidbody = GetParent()->GetComponent<RigidBody2D>();

		Vector2D velocity = mRigidbody->GetVelocity();

		if (velocity.x > 5.0f)
			ChangeState(CHARACTER_RIGHT);
		else if (velocity.x < -5.0f)
			ChangeState(CHARACTER_LEFT);
		else if (velocity.y < -5.0f)
			ChangeState(CHARACTER_FORWARD);
		else if (velocity.y > 5.0f)
			ChangeState(CHARACTER_BACK);
		else
			ChangeState(CHARACTER_IDLE);

	}
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
	case CHARACTER_FORWARD:
		break;
	case CHARACTER_RIGHT:
		break;
	case CHARACTER_LEFT:
		break;
	case CHARACTER_BACK:
		break;
	case CHARACTER_IDLE:
		break;
	default:
		break;
	}
}

void AnimatorCharacter::During(CharacterAnimStates state, float deltaTime)
{
	if (state != CHARACTER_IDLE && state != CHARACTER_ATTACK)
	{
		mAnimations[(int)state]->Update(deltaTime);
	}
	else if(state == CHARACTER_IDLE)
	{
		if (!((int)GetTimeInCurrentState() % 6))
		{
			switch (GetPreviousState())
			{
			case CHARACTER_FORWARD:
				mAnimations[4]->Update(deltaTime);
				break;
			case CHARACTER_BACK:
				mAnimations[5]->Update(deltaTime);
				break;
			case CHARACTER_LEFT:
				mAnimations[6]->Update(deltaTime);
				break;
			case CHARACTER_RIGHT:
				mAnimations[7]->Update(deltaTime);
				break;
			case CHARACTER_IDLE:
				mAnimations[7]->Update(deltaTime);
				break;
			case CHARACTER_ATTACK:
				mAnimations[4]->Update(deltaTime);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		switch (GetPreviousState())
		{
		case CHARACTER_FORWARD:
			mAnimations[8]->Update(deltaTime);
			break;
		case CHARACTER_BACK:
			mAnimations[9]->Update(deltaTime);
			break;
		case CHARACTER_LEFT:
			mAnimations[10]->Update(deltaTime);
			break;
		case CHARACTER_RIGHT:
			mAnimations[11]->Update(deltaTime);
			break;
		case CHARACTER_IDLE:
			mAnimations[11]->Update(deltaTime);
			break;
		default:
			break;
		}
	}
}

void AnimatorCharacter::Exit(CharacterAnimStates state)
{
	switch (state)
	{
	case CHARACTER_FORWARD:
		break;
	case CHARACTER_RIGHT:
		break;
	case CHARACTER_LEFT:
		break;
	case CHARACTER_BACK:
		break;
	default:
		break;
	}
}

//Create all the animations
void AnimatorCharacter::CreateAnimations()
{
	mAnimations.emplace_back(std::make_unique<Animation>(0, 5, 0.05, mSprite));//Forward
	mAnimations.emplace_back(std::make_unique<Animation>(20, 10, 0.05, mSprite));//Left
	mAnimations.emplace_back(std::make_unique<Animation>(10, 10, 0.05, mSprite));//Right
	mAnimations.emplace_back(std::make_unique<Animation>(5, 5, 0.05, mSprite));//Back
	mAnimations.emplace_back(std::make_unique<Animation>(40, 5, 0.1, mSprite));//idle forward
	mAnimations.emplace_back(std::make_unique<Animation>(45, 5, 0.1, mSprite));//idle back
	mAnimations.emplace_back(std::make_unique<Animation>(35, 5, 0.1, mSprite));//idle left
	mAnimations.emplace_back(std::make_unique<Animation>(30, 5, 0.1, mSprite));//idle right
	mAnimations.emplace_back(std::make_unique<Animation>(50, 10, 0.05, mSprite));//attack forward
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(60, 10, 0.05, mSprite));//attack back
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(70, 10, 0.05, mSprite));//attack left
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(80, 10, 0.05, mSprite));//attack right
	mAnimations.back()->AddObserver(this);

	//mAnimations.emplace_back(std::make_unique<Animation>(0, 4, 0.1, mSprite));//Forward
	//mAnimations.emplace_back(std::make_unique<Animation>(4, 4, 0.1, mSprite));//Left
	//mAnimations.emplace_back(std::make_unique<Animation>(8, 4, 0.1, mSprite));//Right
	//mAnimations.emplace_back(std::make_unique<Animation>(12, 4, 0.1, mSprite));//Back
	//mAnimations.emplace_back(std::make_unique<Animation>(16, 2, 0.1, mSprite));//idle forward
	//mAnimations.emplace_back(std::make_unique<Animation>(18, 2, 0.1, mSprite));//idle back
	//mAnimations.emplace_back(std::make_unique<Animation>(20, 4, 0.1, mSprite));//idle left
	//mAnimations.emplace_back(std::make_unique<Animation>(24, 4, 0.1, mSprite));//idle right
}

void AnimatorCharacter::OnNotify(AnimationNotify notify, int chanel)
{
	switch (notify)
	{
	case AnimationNotify::ANIM_END:
		RevertToPreviousState();
		mIsAttacking = false;
		mAttack->StopAttack();
		break;
	default:
		break;
	}
}

void AnimatorCharacter::OnNotify(AttackEvent notify, int payload)
{
	switch (notify)
	{
	case AttackEvent::ON_ATTACK_BEGIN:
		ChangeState(CHARACTER_ATTACK);
		mIsAttacking = true;
		break;
	case AttackEvent::ON_ATTACK_END:
		break;
	default:
		break;
	}
}
