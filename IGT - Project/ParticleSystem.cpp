#include "ParticleSystem.h"
#include "GameScreenManager.h"

void Particle::Update(float deltaTime)
{
	
}

void ParticleSystem::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	if (mLifeSpan < 0.0f)
		GameScreenManager::GetInstance()->GetCurrentScreen()->RemoveGameOject(this);
	else
	{
		mLifeSpan -= deltaTime;


	}
}
