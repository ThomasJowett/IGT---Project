#pragma once

#include "iComponents.h"
#include <iostream>

class LootCollector
	:public Component
{
public:
	LootCollector(GameObject* parent)
		:Component(parent)
	{
		mValue = 0;
	}

	Component* Clone()override { return new LootCollector(nullptr); }

	int PickUpLoot(int valueOfLoot)
	{
		mValue += valueOfLoot;
		std::cout << mValue << "\n";

		return mValue;
	}
private:
	int mValue;
};