#include "DungeonManager.h"



DungeonManager::DungeonManager(GLuint texture)
	:GameObject("Dungeon Manager", new Transform())
{
	AddComponent<Sprite>(texture, 16, 16, 12, 12);

	CreateDungeon();
}


DungeonManager::~DungeonManager()
{
}

void DungeonManager::Update(float deltaTime)
{
}

void DungeonManager::Render(Shader * shader)
{
	GetTransform()->UpdateWorldMatrix();
	shader->UpdateWorld(GetTransform());

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{

			GameObject::Render(shader);
		}
	}


	//render floor
	//render walls and obstacles top to bottom
	//render ceiling
}

void DungeonManager::CreateDungeon()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			mTiles[i][j] = Random::IntInRange2D(i, j, mSeed, 0, 143);
		}
	}
}
