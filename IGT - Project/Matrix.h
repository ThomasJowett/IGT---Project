#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector.h"

class Matrix4x4
{
public:
	float m[4][4];

	Matrix4x4();
	~Matrix4x4();

	static Matrix4x4 Translate(Vector3D translation);
	static Matrix4x4 Scale(Vector3D scale);
	static Matrix4x4 RotateX(float angle);
	static Matrix4x4 RotateY(float angle);
	static Matrix4x4 RotateZ(float angle);
	static Matrix4x4 Perspective(float fovY, float aspectRatio, float nearDepth, float farDepth);
	static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float nearDepth, float farDepth);
	static Matrix4x4 LookAt(Vector3D eyePosition, Vector3D lookAtPosition, Vector3D up);

	static Vector2D MulVec2(Matrix4x4 matrix, Vector2D vector);
	static Vector3D MulVec3(Matrix4x4 matrix, Vector3D vector);

	Vector2D ToVector2D() const;
	Vector3D ToVector3D() const;

	Matrix4x4 operator*(Matrix4x4 other);
};

#endif // !_MATRIX_H