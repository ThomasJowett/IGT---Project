#pragma once

#include<glew.h>

#include"Vector.h"

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT
};

struct Vertex
{
	Vector3D mPosition;
	Vector2D mTexCoord;

	Vertex()
	{
		mPosition = Vector3D(0, 0, 0);
		mTexCoord = Vector2D(0, 0);
	}

	Vertex(const Vector3D& position, const Vector2D& texCoord)
		:mPosition(position), mTexCoord(texCoord) {}
};