#pragma once
#include "Animator.h"

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
	public Animator<SnakeAnimstates>
{
public:
	AnimatorSnake(GameObject* parent);
	virtual ~AnimatorSnake();

	void Update(float deltaTime)override;
	Component* Clone()override;

	virtual void Enter(SnakeAnimstates state);
	virtual void During(SnakeAnimstates state, float deltaTime);
	virtual void Exit(SnakeAnimstates state);

	void CreateAnimations()override;
};