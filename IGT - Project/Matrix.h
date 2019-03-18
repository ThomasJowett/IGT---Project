#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector.h"
#include "Plane.h"
//#include "Quaternion.h"
#include <string>

class Matrix4x4
{
public:
	float m[4][4];

	Matrix4x4();
	Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	~Matrix4x4() = default;

	static Matrix4x4 Translate(Vector3D translation);
	static Matrix4x4 Scale(Vector3D scale);
	static Matrix4x4 RotateX(float angle);
	static Matrix4x4 RotateY(float angle);
	static Matrix4x4 RotateZ(float angle);
	static Matrix4x4 Rotate(float r, float i, float j, float k);
	//static Matrix4x4 Rotate(Quaternion rotation);
	static Matrix4x4 Perspective(float fovY, float aspectRatio, float nearDepth, float farDepth);
	static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float nearDepth, float farDepth);
	static Matrix4x4 LookAt(Vector3D eyePosition, Vector3D lookAtPosition, Vector3D up);

	static void FrustumPlanes(Plane planes[6], Matrix4x4 M);

	static Vector2D MulVec2(Matrix4x4 matrix, Vector2D vector);
	static Vector3D MulVec3(Matrix4x4 matrix, Vector3D vector);

	Vector2D ToVector2D() const;
	Vector3D ToVector3D() const;

	Vector3D ExtractTranslation() const;
	Vector3D ExtractScale()	const;
	//Quaternion ExtractRotation() const;
	float ExtractRotationX() const;
	float ExtractRotationY() const;
	float ExtractRotationZ() const;

	std::string to_string()const;

	Matrix4x4 operator*(Matrix4x4 other);
	float operator()(size_t row, size_t column);
};

#endif // !_MATRIX_H