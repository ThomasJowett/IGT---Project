#include "Health.h"
#include <iostream>//TODO: Remove 

void Health::TakeDamage(float damage)
{
	if (damage <= 0.0f)
	{
		Heal(-damage);
		return;
	}

	if (mHealth - damage <= 0.0f)
	{
		mHealth = 0.0f;

		Notify(HealthEvent::ON_DEATH, GetParent());

		std::cout << GetParent()->GetName() << " is Dead" << std::endl;
	}
	else
	{
		mHealth -= damage;

		Notify(HealthEvent::ON_TAKE_DAMAGE, GetParent());
	}

	std::cout << mHealth << std::endl;
}

void Health::Heal(float heal)
{
	if (heal < 0.0f)
	{
		TakeDamage(-heal);
		return;
	}

	if (mHealth + heal > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
	else
	{
		mHealth += heal;
	}
}

void Health::AddDamageOverTime(float damage, float length, float interval)
{
	mDamageEffects.push_back(DamageOverTime(damage, length, interval));
}

void Health::SetMaxHealth(float maxHealth)
{
	mMaxHealth = maxHealth;
	if (mHealth > mMaxHealth)
	{
		TakeDamage(mHealth - mMaxHealth);
	}
}

void Health::Update(float deltaTime)
{
	std::vector<DamageOverTime>::iterator it = mDamageEffects.begin();
	while (it != mDamageEffects.end())
	{
		it->currentTime += deltaTime;
		it->sinceLastInterval += deltaTime;

		if (it->sinceLastInterval >= it->interval)
		{
			TakeDamage(it->damage);
			it->sinceLastInterval = 0.0f;
		}

		if (it->currentTime >= it->length)
		{
			it = mDamageEffects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

Health * Health::Clone()
{
	return new Health(nullptr, mMaxHealth, mHealth, mDamageEffects);
}
