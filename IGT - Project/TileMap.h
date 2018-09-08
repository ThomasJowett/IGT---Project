#pragma once
#include "Sprite.h"
#include "GameObject.h"

class TileMap
	:public GameObject
{
public:
	TileMap();
	TileMap(const char* mapfilename, const char* tilesetfilename);
	~TileMap();

	void Update(float deltatime)override;
	void Render(Shader* shader)override;

	bool LoadMap(const char* filename);
	bool LoadTileSet(const char* tileSetfilename, const char* spritefilename);

	int GetTileAt(float x, float y);

private:
	int** mTiles;

	int mTilesWide, mTilesHigh;
	int mTileWidth, mTileHeight;

	Sprite* mTileSet;
	bool** mCollision;
};