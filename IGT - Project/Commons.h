#pragma once

#include<glew.h>

#include"Vector.h"

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL_1,
	SCREEN_OPTIONS
};

enum AnimationStates
{
	ANIM_IDLE,
	ANIM_RUN_LEFT,
	ANIM_RUN_RIGHT,
	ANIM_RUN_UP,
	ANIM_RUN_DOWN,
	ANIM_ATTACK_1,
	ANIM_ATTACK_2
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

struct PhysicsMaterial
{
	float drag;
	float elasticity;
	float friction;
	float angularDrag;
};