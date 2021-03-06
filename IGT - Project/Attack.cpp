#include "Attack.h"
#include "Health.h"
#include "GameScreenManager.h"
#include <iostream>
#include "SoundManager.h"
#include "Collision.h"
#include "Projectile.h"

Attack::Attack(GameObject * parent, float damage, float cooldown)
	:iUpdateable(parent), mDamage(damage), mCoolDown(cooldown)
{
	mIsAttacking = false;

	if(GetParent())
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
	return new Attack(nullptr, mDamage, mCoolDown);
}

bool Attack::BeginAttack(Vector2D direction)
{
	if (!mIsOnCoolDown)
	{
		
		SoundManager::GetInstance()->PlaySoundEffect("SoundEffects/Sword_Swing_001.ogg", -1, 0);
		mIsAttacking = true;
		mIsOnCoolDown = true;

		Notify(AttackEvent::ON_ATTACK_BEGIN, 0);

		mCollider->SetOffset(32.0f * direction);

		std::vector<Collider*> colliders;

		Collision::GetQuadtree()->Retrieve(colliders, mCollider);

		for (Collider* collider : colliders)
		{
			GameObject* gameObject = collider->GetParent();

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
		return true;
	}
	return false;
}

void Attack::StopAttack()
{
	mIsAttacking = false;
}

void Attack::SetParent(GameObject * parent)
{
	Component::SetParent(parent);

	if(GetParent())
		mCollider = GetParent()->GetComponent<Collider>(2);
}

RangedAttack::RangedAttack(GameObject * parent, float damage, float cooldown, GameObject* prefab, Vector2D spawnLocation)
	:Attack(parent, damage, cooldown), mProjectiles(prefab), mSpawnLocation(spawnLocation)
{
	mProjectiles.PreLoadObjects(200);
}

void RangedAttack::Update(float deltaTime)
{
	Attack::Update(deltaTime);
}

RangedAttack * RangedAttack::Clone()
{
	return new RangedAttack(nullptr, mDamage, mCoolDown, mProjectiles.GetPrefab(), mSpawnLocation);
}

bool RangedAttack::BeginAttack(Vector2D direction)
{
	if (!mIsOnCoolDown)
	{
		SoundManager::GetInstance()->PlaySoundEffect("SoundEffects/Bow_Loose.ogg", -1, 0);
		mIsAttacking = true;
		mIsOnCoolDown = true;

		Notify(AttackEvent::ON_ATTACK_BEGIN, 0);

		GameObject* newProjectile = mProjectiles.AquireObject();

		//GameObject* newProjectile = Factory<Prefab>::CreateInstance("Arrow")->GetPrefab();

		newProjectile->GetComponent<Projectile>()->Reset(&mProjectiles, GetParent());

		newProjectile->GetLocalTransform()->mPosition = GetParent()->GetWorldTransform().mPosition + mSpawnLocation.to_Vector3D();

		RigidBody2D* rigidBody = newProjectile->GetComponent<RigidBody2D>();

		rigidBody->SetVelocity(Vector2D(0, 0));

		if ((direction.x && direction.y) == 0.0f)
			direction.x = 1.0f;

		rigidBody->ApplyImpulse(300.0f * direction.GetNormalized());

		GameScreenManager::GetInstance()->GetCurrentScreen()->AddGameObject(newProjectile);

		return true;
	}

	return false;
}
