#pragma once

#include<glew.h>

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

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float x, float y):x(x),y(y){}
};