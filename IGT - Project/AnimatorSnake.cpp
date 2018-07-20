#include "AnimatorSnake.h"



AnimatorSnake::AnimatorSnake(GameObject* parent)
	:Animator(parent)
{
	if(GetParent())
		CreateAnimations();
}


AnimatorSnake::~AnimatorSnake()
{
}

void AnimatorSnake::Update(float deltaTime)
{
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
		if( GetTimeInCurrentState() < mAnimations[4]->GetAnimationLength())
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
	mAnimations.emplace_back(std::make_unique<Animation>(40, 10, 0.1, mSprite));//death
}
