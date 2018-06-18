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
	~Sprite();

	void Render(Shader* shader) override;

	void SetCurrentFrame(unsigned int frame) { mCurrentFrame = frame; }
	unsigned int GetCurrentFrame() { return mCurrentFrame; }

	Component* Clone()override;
private:
	GLuint mTextureID;
	std::vector<Mesh*> mTiles;
	unsigned int mCurrentFrame;

	float mSingleSpriteWidth, mSingleSpriteHeight;
	int mTilesWide, mTilesTall;
};

