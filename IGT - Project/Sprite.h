#pragma once
#include "iComponents.h"
#include "Commons.h"
#include <vector>
#include "Mesh.h"
#include <iostream>

class Sprite :
	public iRenderable
{
public:
	Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall);
	Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight);
	Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, Vector2D offset);
	Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall, Vector2D offset);
	Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall, Matrix4x4 offset);
	~Sprite();

	void Render(Shader* shader) override;

	void SetCurrentFrame(unsigned int frame);
	unsigned int GetCurrentFrame() { return mCurrentFrame; }

	unsigned int GetNumberOfFrames() { return mFrames.size(); }

	Component* Clone()override;

	void SetOffset(Vector2D offset);
private:
	GLuint mTextureID;
	std::vector<Mesh*> mFrames;
	unsigned int mCurrentFrame;

	float mSingleSpriteWidth, mSingleSpriteHeight;
	int mTilesWide, mTilesTall;

	Matrix4x4 mOffset;
};

