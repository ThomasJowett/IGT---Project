#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include"Commons.h"

class Transform
{
public:
	Transform()
	{
		mPosition = Vector2D(0, 0);
		mRotation = 0.0f;
		mScale = Vector2D(1.0f, 1.0f);
	}
	Transform(Vector2D position, float rotation, Vector2D scale)
		:mPosition(position), mRotation(rotation), mScale(scale) {;}
	~Transform() { ; }

	Vector2D mPosition;
	float mRotation;
	Vector2D mScale;
};

#endif // !_TRANSFORM_H