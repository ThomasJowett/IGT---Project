#include "Attack.h"
#include "Health.h"
#include "GameScreenManager.h"
#include <iostream>

Attack::Attack(GameObject * parent, float damage, float cooldown)
	:iUpdateable(parent), mDamage(damage), mCoolDown(cooldown)
{
	mIsAttacking = false;

	mCollider = GetParent()->GetComponent<Collider>(2);
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

		for (auto gameObject : GameScreenManager::GetInstance()->GetCurrentScreen()->GetAllGameObjects())
		{
			if (gameObject != GetParent())
			{
				Health * health = gameObject->GetComponent<Health>();
				if (health != nullptr)
				{
					Collider * otherCollider = gameObject->GetComponent<Collider>();

					if (otherCollider != nullptr)
					{
						float penetrationDepth;
						Vector2D normal;
						if (mCollider->IntersectsCollider(otherCollider, normal, penetrationDepth))
						{
							health->TakeDamage(mDamage);
						}
					}
				}
			}
		}
	}
}

void Attack::StopAttack()
{
	mIsAttacking = false;
}
