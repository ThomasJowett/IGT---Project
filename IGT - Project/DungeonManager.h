#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "RandomGenerator.h"

class DungeonManager :
	public GameObject
{
public:
	DungeonManager(GLuint texture);
	virtual ~DungeonManager();

	void Update(float deltaTime)override;
	void Render(Shader* shader)override;

	int mSeed;
	int mTiles[20][20];

private:
	void CreateDungeon();
	void CreateRoom();
	void CreateCorridor();
};