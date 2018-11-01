#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Collider.h"

class TileMap
	:public GameObject
{
public:
	TileMap();
	TileMap(const char* mapfilename, const char* pallettefilename);
	~TileMap();

	void Update(float deltatime)override;
	void Render(Shader* shader)override;

	bool LoadMap(const char* filename);

	void RedrawMap();

	int GetTileAt(Vector2D position);
	bool GetCollisionAt(Vector2D position);

	Collider* GetCollider() { return mCollider; }
	void SetColliderPosition(Vector2D position);

	int GetTileWidth() { return mTileWidth; }
	int GetTileHeight() { return mTileHeight; }

private:
	int** mBackgroundTiles;
	int** mForegroundTiles;

	int mTilesWide, mTilesHigh;
	int mTileWidth, mTileHeight;

	int mPaletteWidth, mPaletteHeight;

	bool** mCollision;

	Mesh* mBackGround;
	Mesh* mForeground;

	GLuint mTextureID;

	Collider* mCollider;

	Vector2D TextureCoordinatesAtIndex(int index, int tile);
	bool PositionToTileIndex(Vector2D position, unsigned int &X, unsigned int &Y);
};