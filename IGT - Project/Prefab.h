#pragma once
#include <vector>
#include "GameObject.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Collider.h"
#include "RigidBody2D.h"

class Prefab
{
public:

	static std::vector<GameObject*> LoadPrefab(std::string filename);

	std::vector<GameObject*> GetPrefab() const { return mGameObjects; }

protected:
	std::vector<GameObject*> mGameObjects;
};

class BarrelPrefab :public Prefab
{
public:
	BarrelPrefab()
	{
		GameObject* gameObject = new GameObject("Barrel", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/Barrel_Closed.png"), 32, 32, Vector2D(0, 8));
		gameObject->AddComponent<Circle2D>(8, Vector2D());
		gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 300.0f, 0.8f, 0.5f, 1.0f });

		mGameObjects.push_back(gameObject);
	}
};

class ChestPrefab : public Prefab
{
public:
	ChestPrefab()
	{
		GameObject* gameObject = new GameObject("Chest", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/Chest_Closed.png"), 16, 16, Vector2D(0, 8));
		gameObject->AddComponent<Box2D>(16, 10, Vector2D(0, 6));
		gameObject->AddComponent<RigidBody2D>(200, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 600.0f, 0.8f, 0.5f, 1.0f });

		mGameObjects.push_back(gameObject);
	}
};

class CoinsPrefab : public Prefab
{
public:
	CoinsPrefab()
	{

	}
};

class LargeLootPrefab : public Prefab
{
public:
	LargeLootPrefab()
	{
		GameObject* gameObject = new GameObject("LargeLootBag", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/LootBag_Large.png"), 16, 16, Vector2D(0, 8));
		gameObject->AddComponent<Circle2D>(6, Vector2D(0,3));
		gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 300.0f, 0.8f, 0.5f, 1.0f });

		mGameObjects.push_back(gameObject);
	}
};

class MediumLootPrefab : public Prefab
{
public:
	MediumLootPrefab()
	{
		GameObject* gameObject = new GameObject("MediumLootBag", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/LootBag_Medium.png"), 16, 16, Vector2D(0, 8));
		gameObject->AddComponent<Circle2D>(6, Vector2D(0, 3));
		gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 300.0f, 0.8f, 0.5f, 1.0f });

		mGameObjects.push_back(gameObject);
	}
};

class SmallLootPrefab : public Prefab
{
public:
	SmallLootPrefab()
	{
		GameObject* gameObject = new GameObject("SmallLootBag", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/LootBag_Small.png"), 16, 16, Vector2D(0, 8));
		gameObject->AddComponent<Circle2D>(6, Vector2D(0, 3));
		gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 300.0f, 0.8f, 0.5f, 1.0f });

		mGameObjects.push_back(gameObject);
	}
};