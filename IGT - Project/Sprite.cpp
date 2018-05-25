#include "Sprite.h"
#include <iostream>


Sprite::Sprite(GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall)
	: mTextureID(TextureID)
{
	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	float halfHeight = singleSpriteHeight / 2;
	float halfWidth = singleSpriteWidth / 2;
	float inverseTall = (float)1 / tilesTall;
	float inverseWide = (float)1 / tilesWide;

	for (int i = 0; i < tilesTall; i++)
	{
		for (int j = 0; j <tilesWide; j++)
		{
			Vertex vertices[] =
			{
				Vertex(Vector3D(-halfWidth,-halfHeight,0), Vector2D(j*inverseWide, (i + 1) * inverseTall)),
				Vertex(Vector3D(halfWidth,-halfHeight,0), Vector2D((j + 1) * inverseWide, (i + 1) * inverseTall)),
				Vertex(Vector3D(halfWidth,halfHeight,0), Vector2D((j + 1) * inverseWide,i * inverseTall)),
				Vertex(Vector3D(-halfWidth,halfHeight,0), Vector2D(j*inverseWide,i * inverseTall))
			};

			Mesh* mesh = new Mesh(vertices, 4, indices, 6);
			mTiles.push_back(mesh);
		}
	}
}

Sprite::Sprite(GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight)
	: mTextureID(TextureID)
{
	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	float halfHeight = singleSpriteHeight / 2;
	float halfWidth = singleSpriteWidth / 2;

	Vertex vertices[] =
	{
		Vertex(Vector3D(-halfWidth,-halfHeight,0), Vector2D(0,1)),
		Vertex(Vector3D(halfWidth,-halfHeight,0), Vector2D(1, 1)),
		Vertex(Vector3D(halfWidth,halfHeight,0), Vector2D(1,0)),
		Vertex(Vector3D(-halfWidth,halfHeight,0), Vector2D(0,0))
	};

	Mesh* mesh = new Mesh(vertices, 4, indices, 6);
	mTiles.push_back(mesh);
}


Sprite::~Sprite()
{
	glDeleteTextures(GL_TEXTURE_2D, &mTextureID);

	for (Mesh* tile : mTiles)
	{
		delete tile;
	}
}

void Sprite::Render(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	mTiles[mCurrentFrame]->Draw();

	mCurrentFrame++;
	if (mCurrentFrame >= mTiles.size())
		mCurrentFrame = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
