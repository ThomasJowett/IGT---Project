#include "AnimatorSnake.h"

#include "GameScreenManager.h"

AnimatorSnake::AnimatorSnake(GameObject* parent)
	:Animator(parent)
{
	if (GetParent())
	{
		CreateAnimations();

		mRigidbody = GetParent()->GetComponent<RigidBody2D>();
		mAttack = GetParent()->GetComponent<Attack>();
		mAttack->AddObserver(this);

		GetParent()->GetComponent<Health>()->AddObserver(this);
	}
}


AnimatorSnake::~AnimatorSnake()
{
}

void AnimatorSnake::Update(float deltaTime)
{
	if (!mIsAttacking && GetState() != SNAKE_DEATH)
	{
		if (!mRigidbody)
			mRigidbody = GetParent()->GetComponent<RigidBody2D>();

		Vector2D velocity = mRigidbody->GetVelocity();

		if (velocity.x > 5.0f)
			ChangeState(SNAKE_RIGHT);
		else if (velocity.x < -5.0f)
			ChangeState(SNAKE_LEFT);
		else
			ChangeState(SNAKE_IDLE);

	}
	Animator::Update(deltaTime);
}

Component * AnimatorSnake::Clone()
{
	return new AnimatorSnake(nullptr);
}

void AnimatorSnake::Enter(SnakeAnimstates state)
{
	switch (state)
	{
	case SNAKE_IDLE:
		break;
	case SNAKE_TURN_LEFT:
		break;
	case SNAKE_TURN_RIGHT:
		break;
	case SNAKE_LEFT:
		GetParent()->SetFacing(FACING_LEFT);
		break;
	case SNAKE_RIGHT:
		GetParent()->SetFacing(FACING_RIGHT);
		break;
	case SNAKE_ATTACK:
		break;
	case SNAKE_DEATH:
		break;
	default:
		break;
	}
}

void AnimatorSnake::During(SnakeAnimstates state, float deltaTime)
{
	switch (state)
	{
	case SNAKE_IDLE:
		if (!((int)GetTimeInCurrentState() % 4))
			mAnimations[0]->Update(deltaTime);
		break;
	case SNAKE_TURN_LEFT:
		mAnimations[1]->Update(deltaTime);
		break;
	case SNAKE_TURN_RIGHT:
		mAnimations[1]->Update(deltaTime);
		break;
	case SNAKE_LEFT:
		mAnimations[2]->Update(deltaTime);
		break;
	case SNAKE_RIGHT:
		mAnimations[2]->Update(deltaTime);
		break;
	case SNAKE_ATTACK:
		if (GetTimeInCurrentState() < mAnimations[3]->GetAnimationLength())
			mAnimations[3]->Update(deltaTime);
		else
			ChangeState(SNAKE_IDLE);
		break;
	case SNAKE_DEATH:
		if( GetTimeInCurrentState() <= mAnimations[4]->GetAnimationLength())
			mAnimations[4]->Update(deltaTime);
		break;
	default:
		break;
	}
}

void AnimatorSnake::Exit(SnakeAnimstates state)
{
}

void AnimatorSnake::CreateAnimations()
{
	mAnimations.emplace_back(std::make_unique<Animation>(0, 10, 0.1, mSprite));//Idle
	mAnimations.emplace_back(std::make_unique<Animation>(10, 10, 0.1, mSprite));//turn
	mAnimations.emplace_back(std::make_unique<Animation>(20, 10, 0.1, mSprite));//left
	mAnimations.emplace_back(std::make_unique<Animation>(30, 10, 0.1, mSprite));//attack
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(40, 10, 0.1, mSprite));//death
	mAnimations.back()->AddObserver(this);
}

void AnimatorSnake::OnNotify(AnimationNotify notify, int chanel)
{
	switch (notify)
	{
	case AnimationNotify::ANIM_END:
		if (GetState() == SNAKE_ATTACK)
		{
			mIsAttacking = false;
		}
		else if (GetState() == SNAKE_DEATH)
		{
			//GameScreenManager::GetInstance()->GetCurrentScreen()->RemoveGameOject(GetParent());
			//TODO: destroy 
		}
		break;
	default:
		break;
	}
}

void AnimatorSnake::OnNotify(AttackEvent notify, int payload)
{
	switch (notify)
	{
	case AttackEvent::ON_ATTACK_BEGIN:
		ChangeState(SNAKE_ATTACK);
		mIsAttacking = true;
		break;
	case AttackEvent::ON_ATTACK_END:
		//mIsAttacking = false;
		break;
	default:
		break;
	}
}

void AnimatorSnake::OnNotify(HealthEvent notify, GameObject * gameObject)
{
	switch (notify)
	{
	case HealthEvent::ON_DEATH:
		ChangeState(SNAKE_DEATH);
		mIsAttacking = false;
		break;
	default:
		break;
	}
}

void AnimatorSnake::SetParent(GameObject * parent)
{
	Animator::SetParent(parent);

	if (GetParent())
	{
		CreateAnimations();

		mRigidbody = GetParent()->GetComponent<RigidBody2D>();
		mAttack = GetParent()->GetComponent<Attack>();
		mAttack->AddObserver(this);

		GetParent()->GetComponent<Health>()->AddObserver(this);
	}
}
