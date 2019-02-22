#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Collider.h"
#include <string>
#include <unordered_map>
#include "Prefab.h"

struct SpawnRoom
{
	Vector2D position;
	float width, height;

	std::unordered_map<Prefab*, int> prefabs;
};

class TileSet
{
public:
	TileSet(const char * filename);
	~TileSet();

	int GetPaletteWidth() { return mPaletteWidth; }
	int GetPalatteHeight() { return mPaletteHeight; }

	GLuint GetTextureID() { return mTextureID; }
private:
	int mPaletteWidth, mPaletteHeight;

	GLuint mTextureID;
};

class TileMap
	:public GameObject
{
public:
	TileMap();
	TileMap(const char* mapfilename);
	TileMap(int ** backgroundTiles, int ** foregroundTiles, bool ** collision,
		int tileHeight, int tileWidth, const char* tileSetName);
	~TileMap();

	void Update(float deltatime)override;
	void Render(Shader* shader)override;


	bool LoadMap(std::string filename);

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

	std::unordered_map<std::string, SpawnRoom> GetSpawnRooms() { return mSpawnRooms; }

private:
	//Tile indexes
	int** mBackgroundTiles;
	int** mForegroundTiles;

	//Tilemap dimensions
	unsigned int mTilesWide, mTilesHigh;
	int mTileWidth, mTileHeight;

	//The appearance of the tilemap
	TileSet* mTileSet;

	bool** mCollision;

	Mesh* mBackGround;
	Mesh* mForeground;

	Collider* mCollider;

	std::vector<Vector2D> mPlayerStarts;

	std::unordered_map<std::string, SpawnRoom> mSpawnRooms;

	Vector2D TextureCoordinatesAtIndex(int index, int tile);
};