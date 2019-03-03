#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Commons.h"
#include "Vector.h"
#include "Matrix.h"

class Transform
{
public:
	Transform()
	{
		mPosition = Vector3D(0, 0, 0);
		mRotation = 0.0f;
		mScale = Vector2D(1.0f, 1.0f);
	}
	Transform(Vector3D position, float rotation, Vector2D scale)
		:mPosition(position), mRotation(rotation), mScale(scale) {;}

	Transform(Matrix4x4 transformationMatrix)
	{
		mWorldMatrix = transformationMatrix;

		mPosition = transformationMatrix.ExtractTranslation();
		mRotation = transformationMatrix.ExtractRotationZ();
		mScale = transformationMatrix.ExtractScale();
	}
	~Transform() { ; }

	Matrix4x4 GetWorldMatrix() const { return mWorldMatrix; }

	void UpdateWorldMatrix()
	{
		Matrix4x4 scale = Matrix4x4::Scale(Vector3D(mScale.x, mScale.y, 1.0f));
		Matrix4x4 rotation = Matrix4x4::RotateZ(mRotation);
		Matrix4x4 translation = Matrix4x4::Translate(mPosition);

		mWorldMatrix = translation * rotation * scale;
	}

	Vector3D mPosition;
	float mRotation;
	Vector2D mScale;

private:
	Matrix4x4 mWorldMatrix;
};

#endif // !_TRANSFORM_H