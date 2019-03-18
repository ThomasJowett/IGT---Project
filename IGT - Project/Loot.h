#pragma once
#include "iComponents.h"
#include "Collider.h"
#include "Messaging.h"

class Loot
	:public Component, public Observer< OverlapEvent, GameObject*>
{
public:
	Loot(GameObject* parent, int value);

	Component* Clone()override;

	void OnNotify(OverlapEvent event, GameObject* gameObject)override;

	void SetParent(GameObject* parent)override;

private:
	int mValue;
	bool mPickedUp = false;
};