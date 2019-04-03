#pragma once

#include "Factory.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Collider.h"
#include "RigidBody2D.h"
#include "Camera.h"
#include "TextRender.h"
#include "AnimatorCharacter.h"
#include "AnimatorSnake.h"
#include "AnimatorGoblin.h"
#include "Health.h"
#include "Attack.h"
#include "AIController.h"
#include "Loot.h"
#include "LootCollector.h"
#include "Destructible.h"

class Prefab
{
public:
	GameObject* GetPrefab() const { return mGameObject; }

protected:
	GameObject* mGameObject;
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

//Lootable prefabs ------------------------------------------------
class BarrelPrefab :public Prefab
{
public:
	BarrelPrefab()
	{
		mGameObject = new GameObject("Barrel", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/Barrel_Closed.png"), 32, 32, Vector2D(0, 8));
		mGameObject->AddComponent<Circle2D>(8, Vector2D());
		mGameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 300.0f, 0.8f, 0.5f, 1.0f });
		mGameObject->AddComponent<Health>(1.0f, "SoundEffects/HitWood.ogg", "SoundEffects/HitWood.ogg");
		mGameObject->AddComponent<Destructible>();
	}

private:
	static DerivedRegister<BarrelPrefab> reg;
};

class ChestPrefab : public Prefab
{
public:
	ChestPrefab()
	{
		mGameObject = new GameObject("Chest", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/Chest_Closed.png"), 16, 16, Vector2D(0, 8));
		mGameObject->AddComponent<Box2D>(16, 10, Vector2D(0, 6));
		mGameObject->AddComponent<RigidBody2D>(200, Vector2D(0, 0), 1, 0, PhysicsMaterial{ 600.0f, 0.8f, 0.5f, 1.0f });
	}
private:
	static DerivedRegister<ChestPrefab> reg;
};

class CoinsPrefab : public Prefab
{
public:
	CoinsPrefab()
	{
		mGameObject = new GameObject("Coins", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/Coins.png"), 16, 16, Vector2D(0, 8));
		mGameObject->AddComponent<Circle2D>(8, Vector2D(), true);
		mGameObject->AddComponent<Loot>(1);
	}

private:
	static DerivedRegister<CoinsPrefab> reg;
};

class LargeLootPrefab : public Prefab
{
public:
	LargeLootPrefab()
	{
		mGameObject = new GameObject("LargeLootBag", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/LootBag_Large.png"), 16, 16, Vector2D(0, 8));
		mGameObject->AddComponent<Circle2D>(6, Vector2D(0, 3), true);
		mGameObject->AddComponent<Loot>(5);
	}
private:
	static DerivedRegister<LargeLootPrefab> reg;
};

class MediumLootPrefab : public Prefab
{
public:
	MediumLootPrefab()
	{
		mGameObject = new GameObject("MediumLootBag", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/LootBag_Medium.png"), 16, 16, Vector2D(0, 8));
		mGameObject->AddComponent<Circle2D>(6, Vector2D(0, 3), true);
		mGameObject->AddComponent<Loot>(2);
	}
private:
	static DerivedRegister<MediumLootPrefab> reg;
};

class SmallLootPrefab : public Prefab
{
public:
	SmallLootPrefab()
	{
		mGameObject = new GameObject("SmallLootBag", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("Images/LootBag_Small.png"), 16, 16, Vector2D(0, 8));
		mGameObject->AddComponent<Circle2D>(6, Vector2D(0, 3), true);
		mGameObject->AddComponent<Loot>(1);
	}
private:
	static DerivedRegister<SmallLootPrefab> reg;
};

//Enemy prefabs----------------------------------------------------
class SlimePrefab : public Prefab
{
public:
	SlimePrefab()
	{
		mGameObject = new GameObject("Slime", new Transform());
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Slime.png"), 32, 32, 4, 10, Vector2D(0, 16));
		mGameObject->AddComponent<Circle2D>(8, Vector2D());
		mGameObject->AddComponent<Circle2D>(16, Vector2D(), true, false);
		mGameObject->AddComponent<Attack>(7.0f, 1.0f);
		mGameObject->AddComponent<Health>(20.0f, "SoundEffects/HitSlime.ogg", "SoundEffects/HitSlime.ogg");
		mGameObject->AddComponent<AnimatorGoblin>();
		mGameObject->AddComponent<AIController>();
	}
private:
	static DerivedRegister<SlimePrefab> reg;
};

class SnakePrefab : public Prefab
{
public:
	SnakePrefab()
	{
		mGameObject = new GameObject("Snake", new Transform());
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Circle2D>(8, Vector2D());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/snake spritesheet calciumtrice.png"), 32, 32, 10, 5, Vector2D(0, 16));
		mGameObject->AddComponent<Circle2D>(16, Vector2D(), true, false);
		mGameObject->AddComponent<Attack>(7.0f, 1.0f);
		mGameObject->AddComponent<Health>(20.0f, "SoundEffects/HitSlime.ogg", "SoundEffects/HitSlime.ogg");
		mGameObject->AddComponent<AnimatorSnake>();
		mGameObject->AddComponent<AIController>();
	}
private:
	static DerivedRegister<SnakePrefab> reg;
};

class BatPrefab : public Prefab
{
public:
	BatPrefab()
	{
		mGameObject = new GameObject("Bat", new Transform());
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Circle2D>(8, Vector2D());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Bat.png"), 32, 32, 10, 5, Vector2D(0, 16));
		mGameObject->AddComponent<Circle2D>(8, Vector2D(), true, false);
		mGameObject->AddComponent<Attack>(5.0f, 1.0f);
		mGameObject->AddComponent<Health>(20.0f, "SoundEffects/RatHurt.ogg", "SoundEffects/RatHurt.ogg");
		mGameObject->AddComponent<AnimatorSnake>();
		mGameObject->AddComponent<AIController>();
	}
private:
	static DerivedRegister<BatPrefab> reg;
};

class RatPrefab : public Prefab
{
public:
	RatPrefab()
	{
		mGameObject = new GameObject("Rat", new Transform());
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Circle2D>(8, Vector2D());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Rat.png"), 32, 32, 10, 5, Vector2D(0, 16));
		mGameObject->AddComponent<Circle2D>(8, Vector2D(), true, false);
		mGameObject->AddComponent<Attack>(5.0f, 1.0f);
		mGameObject->AddComponent<Health>(20.0f, "SoundEffects/RatHurt.ogg", "SoundEffects/RatHurt.ogg");
		mGameObject->AddComponent<AnimatorSnake>();
		mGameObject->AddComponent<AIController>();
	}
private:
	static DerivedRegister<RatPrefab> reg;
};

class GoblinPrefab : public Prefab
{
public:
	GoblinPrefab()
	{
		mGameObject = new GameObject("Goblin", new Transform());
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Circle2D>(8, Vector2D());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Goblin.png"), 48, 48, 4, 10, Vector2D(0, 24));
		mGameObject->AddComponent<Circle2D>(8, Vector2D(), true, false);
		mGameObject->AddComponent<Attack>(5.0f, 1.0f);
		mGameObject->AddComponent<Health>(20.0f, "SoundEffects/GoblinHurt.ogg", "SoundEffects/GoblinHurt.ogg");
		mGameObject->AddComponent<AnimatorGoblin>();
		mGameObject->AddComponent<AIController>();
	}
private:
	static DerivedRegister<GoblinPrefab> reg;
};

//Playable Character prefabs--------------------------------------
class BarbarianCharacterPrefab : public Prefab
{
public:
	BarbarianCharacterPrefab()
	{
		mGameObject = new GameObject("BarbarianCharacter", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Barbarian.png"), 64, 64, 10, 10, Vector2D(0, 32));
		//gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
		mGameObject->AddComponent<Box2D>(20, 10, Vector2D(0, 0));//physics
		mGameObject->AddComponent<Circle2D>(20, Vector2D(0, 0), true, false, CollisionChannel::PLAYER);//attack volume
		mGameObject->AddComponent<Circle2D>(50, Vector2D(0, 0), true, true, CollisionChannel::ENEMY);//enemy agro range
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Attack>(25.0f, 2.0f);
		mGameObject->AddComponent<AnimatorCharacter>();
		mGameObject->AddComponent<Health>(100.0f, "SoundEffects/HitSlime.ogg", "SoundEffects/HitSlime.ogg");
		mGameObject->AddComponent<LootCollector>();
	}
private:
	static DerivedRegister<BarbarianCharacterPrefab> reg;
};

class ArcherCharacterPrefab : public Prefab
{
public:
	ArcherCharacterPrefab()
	{
		mGameObject = new GameObject("ArcherCharacter", new Transform());
		mGameObject->AddComponent<Sprite>(Texture2D::GetTexture2D("SpriteSheets/Archer.png"), 64, 64, 10, 10, Vector2D(0, 32));
		//gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
		mGameObject->AddComponent<Box2D>(20, 10, Vector2D(0, 0));
		mGameObject->AddComponent<Circle2D>(20, Vector2D(0, 0), true, false);
		mGameObject->AddComponent<Circle2D>(50, Vector2D(0, 0), true, true, CollisionChannel::ENEMY);
		mGameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, PhysicsMaterial{ 30.0f, 0.8f, 0.5f, 10.0f });
		mGameObject->AddComponent<Attack>(25.0f, 2.0f);
		mGameObject->AddComponent<AnimatorCharacter>();
		mGameObject->AddComponent<Health>(100.0f, "SoundEffects/HitSlime.ogg", "SoundEffects/HitSlime.ogg");
		mGameObject->AddComponent<LootCollector>();
	}
private:
	static DerivedRegister<ArcherCharacterPrefab> reg;
};