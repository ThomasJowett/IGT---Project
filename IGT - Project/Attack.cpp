#include "Attack.h"
#include <iostream>

Attack::Attack(GameObject * parent, float damage, float cooldown)
	:iUpdateable(parent), mDamage(damage), mCoolDown(cooldown)
{
	mIsAttacking = false;
}

void Attack::Update(float deltaTime)
{
	if (mIsOnCoolDown)
	{
		if (mCurrentTime <= 0.0f)
		{
			mCurrentTime = mCoolDown;
			mIsOnCoolDown = false;
			Notify(AttackEvent::ON_ATTACK_END, 0);
		}
		else
		{
			mCurrentTime -= deltaTime;
		}
	}
}

Attack * Attack::Clone()
{
	return new Attack(nullptr, mDamage, mCurrentTime);
}

void Attack::BeginAttack()
{
	if (!mIsOnCoolDown)
	{
		std::cout << "attacking " << mDamage << std::endl;
		mIsAttacking = true;
		mIsOnCoolDown = true;

		Notify(AttackEvent::ON_ATTACK_BEGIN, 0);
	}
}

void Attack::StopAttack()
{
	mIsAttacking = false;
}
