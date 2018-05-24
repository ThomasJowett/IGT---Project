#ifndef _VECTOR_H
#define _VECTOR_H

#include <string>
#include <math.h>

class Vector3D
{
public:
	float x;
	float y;
	float z;

	Vector3D()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
	//Vector3D(Vector2D v, float z) : x(v.x), y(v.y), z(z) {}

	~Vector3D() = default;

	float const Magnitude()
	{
		return sqrt(SqrMagnitude());
	}

	float const SqrMagnitude()
	{
		return ((x*x) + (y*y) + (z*z));
	}

	Vector3D const GetNormalized()
	{
		float magnitude = Magnitude();
		return Vector3D(x / magnitude, y / magnitude, z / magnitude);
	}

	void Normalize()
	{
		Vector3D normalized = GetNormalized();

		x = normalized.x;
		y = normalized.y;
		z = normalized.z;
	}

	std::string to_string()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z);
	}

	//Static----------------------------------------------------------------
	static Vector3D Cross(Vector3D v1, Vector3D v2)
	{
		Vector3D cross;
		cross.x = (v1.y*v2.z) - (v1.z*v2.y);
		cross.y = -((v1.x*v2.z) - (v1.z*v2.x));
		cross.z = (v1.x*v2.y) - (v1.y*v2.x);
		return cross;
	}

	static float Dot(Vector3D v1, Vector3D v2)
	{
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	}

	static Vector3D Reflect(Vector3D v, Vector3D normal)
	{
		normal.Normalize();
		return v - (normal * (2.0f * Dot(v, normal)));
	}

	static Vector3D Lerp(Vector3D v1, Vector3D v2, float alpha)
	{
		return (v1 * alpha) + (v2 * (1 - alpha));
	}

	//Operators----------------------------------------------------------------
	Vector3D operator*(float scaler)
	{
		return Vector3D(x * scaler, y * scaler, z * scaler);
	}

	Vector3D operator/(float scaler)
	{
		return Vector3D(x / scaler, y / scaler, z / scaler);
	}

	Vector3D operator+(const Vector3D & other)
	{
		return Vector3D(x + other.x, y + other.y, z + other.z);
	}

	Vector3D operator-(const Vector3D & other)
	{
		return Vector3D(x - other.x, y - other.y, z - other.z);
	}

	Vector3D operator+=(const Vector3D & other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3D operator-=(const Vector3D & other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	bool operator==(const Vector3D & other)
	{
		if (x == other.x && y == other.y && z == other.z)
			return true;
		else
			return false;
	}

	bool operator!=(const Vector3D & other)
	{
		if (x == other.x && y == other.y && z == other.z)
			return false;
		else
			return true;
	}

	Vector3D operator=(const Vector3D & other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

class Vector2D
{
public:
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float x, float y) :x(x), y(y) {}

	float const Magnitude() 
	{ 
		return sqrt(SqrMagnitude());
	}

	float const SqrMagnitude() 
	{
		return ((x*x) + (y*y));
	}

	Vector2D const GetNormalized() 
	{
		float magnitude = Magnitude();
		return Vector2D(x / magnitude, y / magnitude);
	}

	void Normalize()
	{
		Vector2D normalized = GetNormalized();
		x = normalized.x;
		y = normalized.y;
	}

	std::string to_string()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y);
	}

	//Static----------------------------------------------------------------
	static float Angle(Vector2D v1, Vector2D v2)
	{
		return acos(Dot(v1, v2)); 
	}

	static float Distance(Vector2D v1, Vector2D v2) 
	{ 
		return (v1 - v2).Magnitude(); 
	}

	static float Dot(Vector2D v1, Vector2D v2)
	{
		return v1.x*v2.x + v1.y*v2.y; 
	}

	static Vector2D Lerp(Vector2D v1, Vector2D v2, float alpha) 
	{ 
		return (v1 * alpha) + (v2 * (1 - alpha)); 
	}

	static Vector2D Reflect(Vector2D v, Vector2D normal)
	{
		Vector2D n = normal.GetNormalized();
		return v - (n*(2.0f * Dot(v, n)));
	}

	//Operators--------------------------------------------------------------
	Vector2D operator*(float scaler)
	{
		return Vector2D(x * scaler, y *scaler);
	}

	Vector2D operator/(float scaler)
	{
		return Vector2D(x / scaler, y / scaler);
	}

	Vector2D operator+(const Vector2D& other)
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator-(const Vector2D& other)
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D operator+=(const Vector2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D operator-=(const Vector2D& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	bool operator==(const Vector2D& other)
	{
		return (x == other.x && y == other.y);
	}

	bool operator!=(const Vector2D& other)
	{
		return !(x == other.x && y == other.y);
	}

	Vector2D operator=(const Vector2D& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
};

#endif // !_VECTOR_H