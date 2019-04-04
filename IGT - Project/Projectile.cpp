#include "Projectile.h"
#include "Health.h"
#include "GameScreenManager.h"

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
	}
}

void Projectile::SetParent(GameObject * parent)
{
	iUpdateable::SetParent(parent);

	if (GetParent())
	{
		GetParent()->GetComponent<Collider>()->AddObserver(this);
	}
}

void Projectile::OnNotify(OverlapEvent event, Collider * collider)
{
	Health* health = collider->GetParent()->GetComponent<Health>();

	if (health)
	{
		health->TakeDamage(mDamage);
	}
	
	Destroy();
}

void Projectile::Destroy()
{
	if (mObjectPool)
	{
		mObjectPool->ReturnObjectToPool(GetParent());

		GetParent()->SetActive(false);
	}
	else
	{
		GameScreenManager::GetInstance()->GetCurrentScreen()->RemoveGameOject(GetParent());
	}
}
