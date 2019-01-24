#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Collider.h"
#include <string>

class TileMap
	:public GameObject
{
public:
	TileMap();
	TileMap(const char* mapfilename);
	TileMap(int ** backgroundTiles, int ** foregroundTiles, bool ** collision,
		int tileHeight, int tileWidth, int paletteWidth, int paletteHeight, const char* paletteFilename);
	~TileMap();

	void Update(float deltatime)override;
	void Render(Shader* shader)override;


	bool LoadMap(std::string filename);

	bool LoadTileSet(const char* filename);

	void RedrawMap();

	int GetTileAt(Vector2D position);
	bool GetCollisionAt(Vector2D position);
	bool GetCollisionAt(int X, int Y);

	Collider* GetCollider() { return mCollider; }
	int SetColliderPosition(Vector2D position);

	int GetTileWidth() { return mTileWidth; }
	int GetTileHeight() { return mTileHeight; }

	Vector2D GetPlayerStart(int ID) { return mPlayerStarts[ID]; }

	bool PositionToTileIndex(Vector2D position, unsigned int &X, unsigned int &Y);
	bool TileIndexToPosition(unsigned int X, unsigned int Y, Vector2D& position);

private:
	int** mBackgroundTiles;
	int** mForegroundTiles;

	unsigned int mTilesWide, mTilesHigh;
	int mTileWidth, mTileHeight;

	int mPaletteWidth, mPaletteHeight;

	bool** mCollision;

	Mesh* mBackGround;
	Mesh* mForeground;

	GLuint mTextureID;

	Collider* mCollider;

	std::vector<Vector2D> mPlayerStarts;

	Vector2D TextureCoordinatesAtIndex(int index, int tile);
	
};