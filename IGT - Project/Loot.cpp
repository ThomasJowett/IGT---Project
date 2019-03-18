#include "Loot.h"
#include "LootCollector.h"
#include "SoundManager.h"

Loot::Loot(GameObject * parent, int value)
	:Component(parent), mValue(value)
{
	if (GetParent())
	{
		GetParent()->GetComponent<Collider>()->AddObserver(this);
	}
}

Component * Loot::Clone()
{
	return new Loot(nullptr, mValue);
}

void Loot::OnNotify(OverlapEvent event, GameObject * gameObject)
{
	LootCollector* collector = gameObject->GetComponent<LootCollector>();

	if (!mPickedUp)
	{
		switch (event)
		{
		case OverlapEvent::BEGIN_OVERLAP:

			if (collector)
			{
				mPickedUp = true;
				collector->PickUpLoot(mValue);
				SoundManager::GetInstance()->PlaySoundEffect("SoundEffects/yay.ogg", -1, 0);
				//TODO: destroy self
			}

			break;
		case OverlapEvent::END_OVERLAP:
			break;
		default:
			break;
		}
	}
}

void Loot::SetParent(GameObject * parent)
{
	Component::SetParent(parent);

	if (GetParent())
	{
		GetParent()->GetComponent<Collider>()->AddObserver(this);
	}
}
