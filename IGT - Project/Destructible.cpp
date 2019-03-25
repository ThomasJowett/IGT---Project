#include "Destructible.h"
#include "SoundManager.h"


Destructible::Destructible(GameObject* parent)
	:Component(parent)
{
	if (GetParent())
	{
		GetParent()->GetComponent<Health>()->AddObserver(this);
	}
}


Destructible::~Destructible()
{
}

void Destructible::OnNotify(HealthEvent event, GameObject * gameObject)
{
	switch (event)
	{
	case HealthEvent::ON_DEATH:
		break;
	case HealthEvent::ON_TAKE_DAMAGE:
		break;
	default:
		break;
	}
}

void Destructible::SetParent(GameObject * parent)
{
	Component::SetParent(parent);

	if (GetParent())
	{
		GetParent()->GetComponent<Health>()->AddObserver(this);
	}
}

Destructible * Destructible::Clone()
{
	return new Destructible(nullptr);
}
