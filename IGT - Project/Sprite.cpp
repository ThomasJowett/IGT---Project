#include "Sprite.h"
#include <iostream>
#include "GeometryGenerator.h"

//Creates a tile sheet from the texture
Sprite::Sprite(GameObject* parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	mTilesTall(tilesTall), mTilesWide(tilesWide),
	iRenderable(parent)
{
	mFrames = Geometry::CreateQuadList(singleSpriteWidth, singleSpriteHeight, tilesTall, tilesWide);
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

	Mesh* mesh = Geometry::CreateQuad(singleSpriteWidth, singleSpriteHeight);
	mFrames.push_back(mesh);

	mCurrentFrame = 0;

	mOffset = Matrix4x4();
}

//Single tile with an offset
Sprite::Sprite(GameObject * parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, Vector2D offset)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	iRenderable(parent)
{
	mTilesTall = 1;
	mTilesWide = 1;

	Mesh* mesh = Geometry::CreateQuad(singleSpriteWidth, singleSpriteHeight);

	mFrames.push_back(mesh);

	mCurrentFrame = 0;

	mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0));
}

//multiple tiles with offset
Sprite::Sprite(GameObject * parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall, Vector2D offset)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	mTilesTall(tilesTall), mTilesWide(tilesWide),
	iRenderable(parent)
{
	mFrames = Geometry::CreateQuadList(singleSpriteWidth, singleSpriteHeight, tilesTall, tilesWide);
	mCurrentFrame = 0;

	mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0));
}

//for cloning
Sprite::Sprite(GameObject * parent, GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall, 
	Matrix4x4 offset, Vector3D tint, float opacity)
	: mTextureID(TextureID), mSingleSpriteWidth(singleSpriteWidth), mSingleSpriteHeight(singleSpriteHeight),
	mTilesTall(tilesTall), mTilesWide(tilesWide), mTint(tint), mOpacity(opacity),
	iRenderable(parent)
{
	mFrames = Geometry::CreateQuadList(singleSpriteWidth, singleSpriteHeight, tilesTall, tilesWide);
	mCurrentFrame = 0;

	mOffset = offset;
}


Sprite::~Sprite()
{
	for (std::vector< Mesh* >::iterator it = mFrames.begin(); it != mFrames.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
	mFrames.clear();
}

//Draws the texture to the screen
void Sprite::Render(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	if (GetParent())
	{
		shader->UpdateMatrixUniform(MODEL_U, GetParent()->GetWorldMatrix() * mOffset, true);
	}
	else
	{
		shader->UpdateMatrixUniform(MODEL_U, Matrix4x4::Translate(Vector3D(0.0f,0.0f, 0.0f)) * mOffset, true);
	}
	shader->Updatefloat4(mTint.x, mTint.y, mTint.z, mOpacity);

	mFrames[mCurrentFrame]->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (GetParent())
	{
		shader->UpdateMatrixUniform(MODEL_U, GetParent()->GetWorldMatrix(), true);
	}
	
}

void Sprite::SetCurrentFrame(unsigned int frame)
{
	if (frame < mFrames.size())
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
	return new Sprite(nullptr, mTextureID, mSingleSpriteWidth, mSingleSpriteHeight, mTilesWide, mTilesTall, mOffset, mTint, mOpacity);
}

void Sprite::SetOffset(Vector2D offset)
{
	mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0));
}
