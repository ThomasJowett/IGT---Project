#pragma once
#include "iComponents.h"
#include "GameObject.h"
#include "ObjectPool.h"
#include <map>

class Particle:public iUpdateable
{
public:

	void Update(float deltaTime)override;

private:
	float mLifeTime;
	float mCurrentTime;
};

class ParticleSystem : public GameObject
{
public:
	void Update(float deltaTime)override;

	void Activate();
	void Deactivate();

private:
	std::map<GameObject*, float> mParticles;
	ObjectPool<GameObject*> mParticlePool;

	float mLifeSpan;
	float mSpawnRate;

	float mParticleStartLife;
};