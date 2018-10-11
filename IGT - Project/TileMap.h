#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Collider.h"

class TileMap
	:public GameObject//, public Collider
{
public:
	TileMap();
	TileMap(const char* mapfilename, const char* tilesetfilename, const char* tilesetCollisionfilename);
	~TileMap();

	void Update(float deltatime)override;
	void Render(Shader* shader)override;

	bool LoadMap(const char* filename);
	bool LoadTileSet(const char* tileSetfilename, const char* spritefilename);

	void RedrawMap();

	int GetTileAt(Vector2D position);

	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth);
	bool ContainsPoint(Vector2D point);
	bool TestAxis(Vector2D axis, float offset, bool greater);

private:
	int** mTiles;

	int mTilesWide, mTilesHigh;
	int mTileWidth, mTileHeight;

	int mTileSetWidth, mTileSetHeight;

	bool** mCollision;

	Mesh* mMesh;
	GLuint mTextureID;

	Vector2D TextureCoordinatesAtIndex(int index, int tile);
};