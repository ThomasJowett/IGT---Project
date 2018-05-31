#pragma once
#include "iComponents.h"
#include "Commons.h"
#include <vector>
#include "Mesh.h"
#include <iostream>

#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \

class Sprite :
	public iRenderable
{
	CLASS_DECLARATION(Sprite)
public:
	Sprite(GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight, int tilesWide, int tilesTall);
	Sprite(GLuint TextureID, float singleSpriteWidth, float singleSpriteHeight);
	~Sprite();

	void Render(Shader* shader) override;

	void SetCurrentFrame(unsigned int frame) { mCurrentFrame = frame; }
	unsigned int GetCurrentFrame() { std::cout << mCurrentFrame;  return mCurrentFrame; }
private:
	GLuint mTextureID;
	std::vector<Mesh*> mTiles;
	unsigned int mCurrentFrame;
};

