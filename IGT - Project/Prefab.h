#pragma once
#include <vector>
#include <map>
#include "GameObject.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Collider.h"
#include "RigidBody2D.h"
#include "Camera.h"
#include "TextRender.h"
#include "AnimatorCharacter.h"
#include "Health.h"
#include "Attack.h"
#include "CameraFollow.h"

class Prefab
{
public:
	std::vector<GameObject*> GetPrefab() const { return mGameObjects; }

protected:
	std::vector<GameObject*> mGameObjects;
};



template<typename T>
struct Factory
{
	typedef std::map<std::string, T*(*)()>map_type;

	static T* CreateInstance(std::string const& name)
	{
		typename map_type::iterator it  = getMap()->find(name);

		return (it == getMap()->end() ? nullptr : it->second());
	}

	
protected:
	static map_type *getMap()
	{
		if (!gLookUpTable) 
		{ 
			gLookUpTable = new map_type;
		}
		return gLookUpTable;
	}
private:
	static map_type * gLookUpTable;
};

template<typename T> Prefab * CreateT() { return new T; }

template<typename T>
struct DerivedRegister : Factory<Prefab>
{
	DerivedRegister(std::string const& name)
	{
		getMap()->insert(std::make_pair(name, &CreateT<T>));
	}
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

private:
	static DerivedRegister<BarrelPrefab> reg;
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
private:
	static DerivedRegister<ChestPrefab> reg;
};

class CoinsPrefab : public Prefab
{
public:
	CoinsPrefab()
	{
		//TODO: coins prefab
	}

private:
	static DerivedRegister<CoinsPrefab> reg;
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
private:
	static DerivedRegister<LargeLootPrefab> reg;
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
private:
	static DerivedRegister<MediumLootPrefab> reg;
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
private:
	static DerivedRegister<SmallLootPrefab> reg;
};

class SlimePrefab : public Prefab
{
public:
	SlimePrefab()
	{
		GameObject* gameObject = new GameObject("Slime", new Transform());

		//TODO: slime prefab

		mGameObjects.push_back(gameObject);
	}
private:
	static DerivedRegister<SlimePrefab> reg;
};

class BarbarianCharacterPrefab : public Prefab
{
public:
	BarbarianCharacterPrefab()
	{
		GameObject* gameObject = new GameObject("BarbarianCharacter", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Barbarian.png"), 64, 64, 10, 10, Vector2D(0, 32));
		//gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
		gameObject->AddComponent<Box2D>(20, 10, Vector2D(0, 0));
		gameObject->AddComponent<Circle2D>(20, Vector2D(0, 0));
		gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		gameObject->AddComponent<Attack>(25.0f, 2.0f);
		gameObject->AddComponent<AnimatorCharacter>();
		gameObject->AddComponent<Health>(100.0f);
		mGameObjects.push_back(gameObject);
	}
};

class ArcherCharacterPrefab : public Prefab
{
public:
	ArcherCharacterPrefab()
	{
		GameObject* gameObject = new GameObject("ArcherCharacter", new Transform());
		gameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Archer.png"), 64, 64, 10, 10, Vector2D(0, 32));
		//gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
		gameObject->AddComponent<Box2D>(20, 10, Vector2D(0, 0));
		gameObject->AddComponent<Circle2D>(20, Vector2D(0, 0));
		gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		gameObject->AddComponent<Attack>(25.0f, 2.0f);
		gameObject->AddComponent<AnimatorCharacter>();
		gameObject->AddComponent<Health>(100.0f);
		mGameObjects.push_back(gameObject);
	}
};