#include "Matrix.h"



Matrix4x4::Matrix4x4()
{
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}


Matrix4x4::~Matrix4x4()
{
}

Matrix4x4 Matrix4x4::Translate(Vector3D translation)
{
	Matrix4x4 result;
	result.m[0][3] = translation.x;
	result.m[1][3] = translation.y;
	result.m[2][3] = translation.z;
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Matrix4x4::Scale(Vector3D scale)
{
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Matrix4x4::RotateX(float angle)
{
	Matrix4x4 result;
	result.m[1][1] = cos(angle);
	result.m[1][2] = -sin(angle);
	result.m[2][1] = sin(angle);
	result.m[2][2] = cos(angle);
	return result;
}

Matrix4x4 Matrix4x4::RotateY(float angle)
{
	Matrix4x4 result;
	result.m[0][0] = cos(angle);
	result.m[2][0] = sin(angle);
	result.m[2][0] = -sin(angle);
	result.m[2][2] = cos(angle);
	return result;
}

Matrix4x4 Matrix4x4::RotateZ(float angle)
{
	Matrix4x4 result;
	result.m[0][0] = cos(angle);
	result.m[0][1] = -sin(angle);
	result.m[1][0] = sin(angle);
	result.m[1][1] = cos(angle);
	return result;
}

Matrix4x4 Matrix4x4::Perspective(float fovY, float aspectRatio, float nearDepth, float farDepth)
{
	float tanHalfFovY = (tan(fovY / 2));

	Matrix4x4 result;
	result.m[0][0] = 1 / (tanHalfFovY * aspectRatio);
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1 / tanHalfFovY;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = farDepth / (nearDepth - farDepth);
	result.m[2][3] = -1.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = -(farDepth * nearDepth) / (farDepth - nearDepth);
	result.m[3][3] = 0.0f;
	return result;
}

Matrix4x4 Matrix4x4::Orthographic(float left, float right, float bottom, float top, float nearDepth, float farDepth)
{
	Matrix4x4 result;
	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = -2 / (farDepth - nearDepth);
	result.m[2][3] = 0.0f;

	result.m[3][0] = -((right + left) / (right - left));
	result.m[3][1] = -((top + bottom) / (top - bottom));
	result.m[3][2] = -((farDepth + nearDepth) / (farDepth - nearDepth));
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Matrix4x4::LookAt(Vector3D eyePosition, Vector3D lookAtPosition, Vector3D up)
{
	Vector3D f = (lookAtPosition - eyePosition).GetNormalized();
	Vector3D s = (Vector3D::Cross(f, up)).GetNormalized();
	Vector3D u = Vector3D::Cross(s, f);

	Matrix4x4 result;

	result.m[0][0] = s.x;
	result.m[0][1] = u.x;
	result.m[0][2] = -f.x;
	result.m[0][3] = 0.0f;

	result.m[1][0] = s.y;
	result.m[1][1] = u.y;
	result.m[1][2] = -f.y;
	result.m[1][3] = 0.0f;

	result.m[2][0] = s.z;
	result.m[2][1] = u.z;
	result.m[2][2] = -f.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = -Vector3D::Dot(s, eyePosition);
	result.m[3][1] = -Vector3D::Dot(up, eyePosition);
	result.m[3][2] = Vector3D::Dot(f, eyePosition);
	result.m[3][3] = 1.0f;

	return result;
}

void Matrix4x4::FrustumPlanes(Plane planes[6], Matrix4x4 matrix)
{
	//left
	planes[0].a = matrix(0,3) + matrix(0,0);
	planes[0].b = matrix.m[1][3] + matrix.m[1][0];
	planes[0].c = matrix.m[2][3] + matrix.m[2][0];
	planes[0].d = matrix.m[3][3] + matrix.m[3][0];

	//right
	planes[1].a = matrix.m[0][3] - matrix.m[0][0];
	planes[1].b = matrix.m[1][3] - matrix.m[1][0];
	planes[1].c = matrix.m[2][3] - matrix.m[2][0];
	planes[1].d = matrix.m[3][3] - matrix.m[3][0];

	//Bottom
	planes[2].a = matrix.m[0][3] + matrix.m[0][1];
	planes[2].b = matrix.m[1][3] + matrix.m[1][1];
	planes[2].c = matrix.m[2][3] + matrix.m[2][1];
	planes[2].d = matrix.m[3][3] + matrix.m[3][1];

	//Top
	planes[3].a = matrix.m[0][3] - matrix.m[0][1];
	planes[3].b = matrix.m[1][3] - matrix.m[1][1];
	planes[3].c = matrix.m[2][3] - matrix.m[2][1];
	planes[3].d = matrix.m[3][3] - matrix.m[3][1];

	//Near
	planes[4].a = matrix.m[0][2];
	planes[4].b = matrix.m[1][2];
	planes[4].c = matrix.m[2][2];
	planes[4].d = matrix.m[3][2];

	//Far
	planes[5].a = matrix.m[0][3] - matrix.m[0][2];
	planes[5].b = matrix.m[1][3] - matrix.m[1][2];
	planes[5].c = matrix.m[2][3] - matrix.m[2][2];
	planes[5].d = matrix.m[3][3] - matrix.m[3][2];

	for (int i = 0; i < 6; ++i)
	{
		planes[i].Normalize();
	}
}

Vector2D Matrix4x4::MulVec2(Matrix4x4 matrix, Vector2D vector)
{
	Vector2D result;
	result.x = (matrix(0,0) * vector.x) + (matrix(0,1) * vector.x) + (matrix(0,3) * vector.x);
	result.y = (matrix(1,0) * vector.y) + (matrix(1,1) * vector.y) + (matrix(1,3) * vector.y);
	return result;
}

Vector3D Matrix4x4::MulVec3(Matrix4x4 matrix, Vector3D vector)
{
	Vector3D result;
	result.x = (matrix(0,0) * vector.x) + (matrix(0,1) * vector.x) + (matrix(0,2) * vector.x) + (matrix(0,3) * vector.x);
	result.y = (matrix(1,0) * vector.y) + (matrix(1,1) * vector.y) + (matrix(1,2) * vector.y) + (matrix(1,3) * vector.y);
	result.z = (matrix(2,0) * vector.z) + (matrix(2,1) * vector.z) + (matrix(2,2) * vector.z) + (matrix(2,3) * vector.z);
	return result;
}

Vector2D Matrix4x4::ToVector2D() const
{
	Vector2D result;
	result.x = m[0][0] + m[0][1] + m[0][3];
	result.y = m[1][0] + m[1][1] + m[1][3];
	return result;
}

Vector3D Matrix4x4::ToVector3D() const
{
	Vector3D result;
	result.x = m[0][0] + m[0][1] + m[0][2] + m[0][3];
	result.y = m[1][0] + m[1][1] + m[1][2] + m[1][3];
	result.z = m[2][0] + m[2][1] + m[2][2] + m[2][3];
	return Vector3D();
}

std::string Matrix4x4::to_string() const
{
	std::string result;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{

			result += std::to_string(m[i][j]);
			if (j != 3)
			{
				result += ", ";
			}

		}
		if (i != 3)
		{
			result += "\n";
		}
	}

	return result;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 other)
{
	Matrix4x4 result;
	// Cache the invariants in registers
	float x = m[0][0];
	float y = m[0][1];
	float z = m[0][2];
	float w = m[0][3];
	// Perform the operation on the first row
	result.m[0][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
	result.m[0][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
	result.m[0][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
	result.m[0][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
	// Repeat for all the other rows
	x = m[1][0];
	y = m[1][1];
	z = m[1][2];
	w = m[1][3];
	result.m[1][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
	result.m[1][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
	result.m[1][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
	result.m[1][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
	x = m[2][0];
	y = m[2][1];
	z = m[2][2];
	w = m[2][3];
	result.m[2][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
	result.m[2][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
	result.m[2][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
	result.m[2][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
	x = m[3][0];
	y = m[3][1];
	z = m[3][2];
	w = m[3][3];
	result.m[3][0] = (other.m[0][0] * x) + (other.m[1][0] * y) + (other.m[2][0] * z) + (other.m[3][0] * w);
	result.m[3][1] = (other.m[0][1] * x) + (other.m[1][1] * y) + (other.m[2][1] * z) + (other.m[3][1] * w);
	result.m[3][2] = (other.m[0][2] * x) + (other.m[1][2] * y) + (other.m[2][2] * z) + (other.m[3][2] * w);
	result.m[3][3] = (other.m[0][3] * x) + (other.m[1][3] * y) + (other.m[2][3] * z) + (other.m[3][3] * w);
	return result;
}

float Matrix4x4::operator()(size_t row, size_t column)
{
	return m[row][column];
}
