#pragma once
#include "iComponents.h"

class Attack
	:public iUpdateable
{
public:
	Attack(GameObject* parent);
	~Attack() = default;

	void Update(float deltaTime) override;

	float mTimeBetweenAttack;
	float mStartTimeBetweenAttack;
};