#pragma once
#include "iComponents.h"
#include "Commons.h"
#include <vector>
#include "Mesh.h"

class Sprite :
	public iRenderable
{
public:
	Sprite(GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall);
	Sprite(GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight);
	~Sprite();

	void Render(Shader* shader) override;
private:
	GLuint TextureID;
	std::vector<Mesh*> mTiles;
	unsigned int mCurrentFrame;
};

