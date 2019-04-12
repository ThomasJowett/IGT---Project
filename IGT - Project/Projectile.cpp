#include "Projectile.h"
#include "Health.h"
#include "GameScreenManager.h"
#include <math.h> //atan2
#include "SoundManager.h"

Projectile::Projectile(GameObject * parent, float damage, float lifeTime)
	:iUpdateable(parent), mDamage(damage), mLifeTime(lifeTime), mInitialLifeTime(lifeTime) 
{
	if (GetParent())
	{
		GetParent()->GetComponent<Collider>()->AddObserver(this);
		mRigidBody = GetParent()->GetComponent<RigidBody2D>();
	}
}

Projectile * Projectile::Clone()
{
	return new Projectile(nullptr, mDamage, mLifeTime);
}

void Projectile::Update(float deltaTime)
{
	if (mLifeTime <= 0.0f)
	{
		Destroy();
	}
	else
	{
		mLifeTime -= deltaTime;

		Vector2D velocity = mRigidBody->GetVelocity();

		float angle = atan2(-velocity.x, velocity.y);

		GetParent()->GetLocalTransform()->mRotation = angle;
	}
}

void Projectile::SetParent(GameObject * parent)
{
	Component::SetParent(parent);

	if (GetParent())
	{
		GetParent()->GetComponent<Collider>()->AddObserver(this);
		mRigidBody = GetParent()->GetComponent<RigidBody2D>();
	}
}

void Projectile::OnNotify(OverlapEvent event, Collider * collider)
{
	if (collider->GetParent() != mInstigator && !collider->GetCollisionChannel() != CollisionChannel::PROJECTILE && !collider->IsTrigger())
	{
		Health* health = collider->GetParent()->GetComponent<Health>();

		if (health)
		{
			health->TakeDamage(mDamage);
		}
		Destroy();

		SoundManager::GetInstance()->PlaySoundEffect("SoundEffects/Arrow_Hit.ogg", -1, 0);
	}
}

void Projectile::Reset(ObjectPool<GameObject>* objectPool, GameObject* instigator)
{
	mObjectPool = objectPool; 
	mLifeTime = mInitialLifeTime;

	mInstigator = instigator;
}

void Projectile::Destroy()
{
	//if (mObjectPool)
	//{
	//	mObjectPool->ReturnObjectToPool(GetParent());
	//
	//	GameScreenManager::GetInstance()->GetCurrentScreen()->RemoveGameObject(GetParent());
	//}
	//else
	{
		GameScreenManager::GetInstance()->GetCurrentScreen()->DestroyGameOject(GetParent());
	}
}
