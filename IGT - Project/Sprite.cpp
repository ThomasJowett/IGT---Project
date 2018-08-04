#include "Sprite.h"
#include <iostream>

//Creates a tile sheet from the texture
Sprite::Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	mTilesTall(tilesTall), mTilesWide(tilesWide),
	iRenderable(parent)
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

	mCurrentFrame = 0;

	mOffset = Matrix4x4();
}

//Creates a single tile from the texture
Sprite::Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	iRenderable(parent) 
{
	mTilesTall = 1;
	mTilesWide = 1;

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

	mCurrentFrame = 0;

	mOffset = Matrix4x4();
}

Sprite::Sprite(GameObject * parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, Vector2D offset)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	iRenderable(parent)
{
	mTilesTall = 1;
	mTilesWide = 1;

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

	mCurrentFrame = 0;

	mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0));
}


Sprite::~Sprite()
{
	for (std::vector< Mesh* >::iterator it = mTiles.begin(); it != mTiles.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
	mTiles.clear();
}

//Draws the texture to the screen
void Sprite::Render(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	shader->UpdateWorld(GetParent()->GetTransform()->GetWorldMatrix() * mOffset);

	mTiles[mCurrentFrame]->Draw();

	if (mCurrentFrame >= mTiles.size())
		mCurrentFrame = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader->UpdateWorld(GetParent()->GetTransform()->GetWorldMatrix());
}

void Sprite::SetCurrentFrame(unsigned int frame)
{
	if (frame < mTiles.size())
	{
		mCurrentFrame = frame;
	}
	else
	{
		std::cerr << "Error: Frame out of bounds in " << GetParent()->GetName() << std::endl;
	}
}

Component * Sprite::Clone()
{
	return new Sprite(nullptr, mTextureID, mSingleSpriteWidth, mSingleSpriteHeight, mTilesWide, mTilesTall);
}
