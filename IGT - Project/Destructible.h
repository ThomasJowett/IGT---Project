#pragma once
#include "iComponents.h"
#include "Health.h"

class Destructible :
	public Component, public Observer<HealthEvent, GameObject*>
{
public:
	Destructible(GameObject* parent);
	~Destructible();

	void OnNotify(HealthEvent event, GameObject* gameObject) override;

	void SetParent(GameObject* parent)override;

	Destructible* Clone()override;
};

