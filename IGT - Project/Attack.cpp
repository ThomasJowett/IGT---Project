#include "Attack.h"

Attack::Attack(GameObject * parent)
	:iUpdateable(parent)
{
}

void Attack::Update(float deltaTime)
{
	if (mTimeBetweenAttack <= 0.0f)
	{
		mTimeBetweenAttack = mStartTimeBetweenAttack;
	}
	else
	{
		mTimeBetweenAttack -= deltaTime;
	}
}
