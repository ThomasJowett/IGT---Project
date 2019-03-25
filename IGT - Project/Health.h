#pragma once
#include "iComponents.h"
#include "Messaging.h"

enum class HealthEvent { ON_DEATH, ON_TAKE_DAMAGE };

struct DamageOverTime
{
	float damage;
	float length;
	float interval;

	float currentTime;
	float sinceLastInterval;

	DamageOverTime(float damage, float length, float interval)
		:damage(damage), length(length), interval(interval)
	{
		currentTime = 0.0f;
		sinceLastInterval = 0.0f;
	}
};

class Health
	:public iUpdateable, public Subject<HealthEvent, GameObject*>
{
public:
	//Max health
	Health(GameObject* parent, float maxHealth, std::string damageSoundEffect, std::string deathSoundEffect)
		:iUpdateable(parent), mMaxHealth(maxHealth)
	{
		mDamageSoundEffect = damageSoundEffect;
		mDeathSoundEffect = deathSoundEffect;
		mHealth = mMaxHealth;
	}
	//Health and max health
	Health(GameObject* parent, float maxHealth, float health)
		:iUpdateable(parent), mMaxHealth(maxHealth)
	{
		if (health > mMaxHealth)
		{
			mHealth = mMaxHealth;
		}
		else
		{
			mHealth = health;
		}
	}

	//For cloning
	Health(GameObject* parent, float maxHealth, float health, std::vector<DamageOverTime> damageEffects, std::string damageSoundEffect, std::string deathSoundEffect)
		:iUpdateable(parent), mMaxHealth(maxHealth), mHealth(health), mDamageEffects(damageEffects), mDamageSoundEffect(damageSoundEffect), mDeathSoundEffect(deathSoundEffect) {}
		
	void TakeDamage(float damage);
	void Heal(float heal);

	void AddDamageOverTime(float damage, float length, float interval);

	float GetHealth() const { return mHealth; }
	float GetMaxHealth() const { return mMaxHealth; }
	float GetHealthAsPercentage() const { return (mHealth / mMaxHealth)*100.0f; }
	float GetHealthAsFraction() const { return mHealth / mMaxHealth; }

	void SetMaxHealth(float maxHealth);

	void Update(float deltaTime) override;

	Health* Clone() override;

private:
	float mHealth;
	float mMaxHealth;

	std::vector<DamageOverTime> mDamageEffects;

	std::string mDamageSoundEffect;
	std::string mDeathSoundEffect;
};