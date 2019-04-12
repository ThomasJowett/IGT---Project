#include "AnimatorGoblin.h"
#include "GameScreenManager.h"

AnimatorGoblin::AnimatorGoblin(GameObject * parent)
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

AnimatorGoblin::~AnimatorGoblin()
{
}

void AnimatorGoblin::Update(float deltaTime)
{
	if (!mIsAttacking)
	{
		if (!mRigidbody)
			mRigidbody = GetParent()->GetComponent<RigidBody2D>();

		Vector2D velocity = mRigidbody->GetVelocity();

		if (velocity.x > 5.0f)
			ChangeState(GOBLIN_RIGHT);
		else if (velocity.x < -5.0f)
			ChangeState(GOBLIN_LEFT);
		else if (velocity.y < -5.0f)
			ChangeState(GOBLIN_FORWARD);
		else if (velocity.y > 5.0f)
			ChangeState(GOBLIN_BACK);
		else
			ChangeState(GOBLIN_IDLE);

	}
	Animator::Update(deltaTime);
}

Component * AnimatorGoblin::Clone()
{
	return new AnimatorGoblin(nullptr);
}

void AnimatorGoblin::Enter(GoblinAnimstates state)
{
	switch (state)
	{
	case GOBLIN_FORWARD:
		break;
	case GOBLIN_RIGHT:
		break;
	case GOBLIN_LEFT:
		break;
	case GOBLIN_BACK:
		break;
	case GOBLIN_IDLE:
		break;
	default:
		break;
	}
}

void AnimatorGoblin::During(GoblinAnimstates state, float deltaTime)
{
	if (state != GOBLIN_IDLE && state != GOBLIN_ATTACK)
	{
		mAnimations[(int)state]->Update(deltaTime);
	}
	else if (state == GOBLIN_IDLE)
	{
		if (!((int)GetTimeInCurrentState() % 6))
		{
			switch (GetPreviousState())
			{
			case GOBLIN_FORWARD:
				mAnimations[4]->Update(deltaTime);
				break;
			case GOBLIN_BACK:
				mAnimations[5]->Update(deltaTime);
				break;
			case GOBLIN_LEFT:
				mAnimations[6]->Update(deltaTime);
				break;
			case GOBLIN_RIGHT:
				mAnimations[7]->Update(deltaTime);
				break;
			case GOBLIN_IDLE:
				mAnimations[7]->Update(deltaTime);
				break;
			case GOBLIN_ATTACK:
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
		case GOBLIN_FORWARD:
			mAnimations[8]->Update(deltaTime);
			break;
		case GOBLIN_BACK:
			mAnimations[9]->Update(deltaTime);
			break;
		case GOBLIN_LEFT:
			mAnimations[10]->Update(deltaTime);
			break;
		case GOBLIN_RIGHT:
			mAnimations[11]->Update(deltaTime);
			break;
		case GOBLIN_IDLE:
			mAnimations[11]->Update(deltaTime);
			break;
		default:
			break;
		}
	}
}

void AnimatorGoblin::Exit(GoblinAnimstates state)
{
	switch (state)
	{
	case GOBLIN_FORWARD:
		break;
	case GOBLIN_LEFT:
		break;
	case GOBLIN_RIGHT:
		break;
	case GOBLIN_BACK:
		break;
	case GOBLIN_IDLE:
		break;
	case GOBLIN_ATTACK:
		break;
	default:
		break;
	}
}

void AnimatorGoblin::CreateAnimations()
{
	mAnimations.emplace_back(std::make_unique<Animation>(0, 4, 0.2, mSprite));//Forward
	mAnimations.emplace_back(std::make_unique<Animation>(4, 4, 0.2, mSprite));//Left
	mAnimations.emplace_back(std::make_unique<Animation>(8, 4, 0.2, mSprite));//Right
	mAnimations.emplace_back(std::make_unique<Animation>(12, 4, 0.2, mSprite));//Back
	mAnimations.emplace_back(std::make_unique<Animation>(16, 2, 0.6, mSprite));//idle forward
	mAnimations.emplace_back(std::make_unique<Animation>(18, 2, 0.6, mSprite));//idle back
	mAnimations.emplace_back(std::make_unique<Animation>(20, 4, 0.6, mSprite));//idle left
	mAnimations.emplace_back(std::make_unique<Animation>(24, 4, 0.6, mSprite));//idle right
	mAnimations.emplace_back(std::make_unique<Animation>(28, 2, 0.2, mSprite));//attack forward
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(30, 2, 0.2, mSprite));//attack back
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(32, 4, 0.2, mSprite));//attack left
	mAnimations.back()->AddObserver(this);
	mAnimations.emplace_back(std::make_unique<Animation>(36, 4, 0.2, mSprite));//attack right
	mAnimations.back()->AddObserver(this);
}

void AnimatorGoblin::OnNotify(AnimationNotify notify, int chanel)
{
	switch (notify)
	{
	case AnimationNotify::ANIM_END:
		if (GetState() == GOBLIN_ATTACK)
		{
			RevertToPreviousState();
			mIsAttacking = false;
			mAttack->StopAttack();
		}
		else if (GetState() == GOBLIN_DEATH)
		{
			GameScreenManager::GetInstance()->GetCurrentScreen()->DestroyGameOject(GetParent());
		}
		break;
	default:
		break;
	}
}

void AnimatorGoblin::OnNotify(AttackEvent notify, int payload)
{
	switch (notify)
	{
	case AttackEvent::ON_ATTACK_BEGIN:
		ChangeState(GOBLIN_ATTACK);
		mIsAttacking = true;
		break;
	case AttackEvent::ON_ATTACK_END:
		break;
	default:
		break;
	}
}

void AnimatorGoblin::SetParent(GameObject * parent)
{
	Animator::SetParent(parent);

	if (GetParent())
	{
		mRigidbody = GetParent()->GetComponent<RigidBody2D>();
		mAttack = GetParent()->GetComponent<Attack>();
		mAttack->AddObserver(this);

		CreateAnimations();
	}
}
