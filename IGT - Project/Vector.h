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

	~Vector3D() = default;

	//Length of the vector
	float const Magnitude()
	{
		return sqrt(SqrMagnitude());
	}

	//Square length of the vector
	float const SqrMagnitude()
	{
		return ((x*x) + (y*y) + (z*z));
	}

	//Gets a vector of same direction with magnitude of 1
	Vector3D const GetNormalized()
	{
		float magnitude = Magnitude();
		return Vector3D(x / magnitude, y / magnitude, z / magnitude);
	}

	//makes the magnitude of the vector 1
	void Normalize()
	{
		Vector3D normalized = GetNormalized();

		x = normalized.x;
		y = normalized.y;
		z = normalized.z;
	}

	//converts vector to a formatted string
	std::string to_string()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z);
	}

	//Static----------------------------------------------------------------

	//returns a vector orthagonal to both v1 and v2
	static Vector3D Cross(Vector3D v1, Vector3D v2)
	{
		Vector3D cross;
		cross.x = (v1.y*v2.z) - (v1.z*v2.y);
		cross.y = -((v1.x*v2.z) - (v1.z*v2.x));
		cross.z = (v1.x*v2.y) - (v1.y*v2.x);
		return cross;
	}

	//returns the sum of the products of v1 and v2
	static float Dot(Vector3D v1, Vector3D v2)
	{
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	}

	//returns a vector that is the reflection v against the normal
	static Vector3D Reflect(Vector3D v, Vector3D normal)
	{
		normal.Normalize();
		return v - (normal * (2.0f * Dot(v, normal)));
	}

	//linearly interpolate between v1 and v2
	static Vector3D Lerp(Vector3D v1, Vector3D v2, float alpha)
	{
		return (v1 * alpha) + (v2 * (1 - alpha));
	}

	//spherically interpolate between v1 and v2
	static Vector3D Slerp(Vector3D v1, Vector3D v2, float alpha)
	{
		float dot = Dot(v1, v2);
		float theta = acos(dot) * alpha;

		Vector3D relativeVec = v2 - v1 * dot;
		relativeVec.Normalize();

		return ((v1 * cos(theta)) + (relativeVec * sin(theta)));
	}

	//Operators----------------------------------------------------------------

	//multiples each component of the vector by the scaler
	Vector3D operator*(float scaler)
	{
		return Vector3D(x * scaler, y * scaler, z * scaler);
	}

	//divides each component of the vector by the scaler
	Vector3D operator/(float scaler)
	{
		return Vector3D(x / scaler, y / scaler, z / scaler);
	}

	//adds the two vectors together
	Vector3D operator+(const Vector3D & other)
	{
		return Vector3D(x + other.x, y + other.y, z + other.z);
	}

	//subtracts v2 from v2
	Vector3D operator-(const Vector3D & other)
	{
		return Vector3D(x - other.x, y - other.y, z - other.z);
	}

	Vector3D operator-(void)const
	{
		return Vector3D(-x, -y, -z);
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
		return (x == other.x && y == other.y && z == other.z);
	}

	bool operator!=(const Vector3D & other)
	{
		return !(x == other.x && y == other.y && z == other.z);
	}

	Vector3D operator=(const Vector3D & other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

inline Vector3D operator*(float scaler, const Vector3D& v)
{
	return Vector3D(scaler * v.x, scaler * v.y, scaler * v.z);
}

inline Vector3D operator/(float scaler, const Vector3D& v)
{
	return Vector3D(scaler / v.x, scaler / v.y, scaler / v.z);
}

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

	Vector2D(Vector3D vector3d) :x(vector3d.x), y(vector3d.y) {}

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

	//set the vectors length to 1
	void Normalize()
	{
		Vector2D normalized = GetNormalized();
		x = normalized.x;
		y = normalized.y;
	}

	//Perpendicular clockwise
	Vector2D Perpendicular(bool clockwise)
	{
		if (clockwise)
			return Vector2D(y, -x);
		else
			return Vector2D(-y, x);
	}

	std::string to_string()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y);
	}

	Vector3D to_Vector3D()
	{
		return Vector3D(x, y, 0);
	}

	//Static----------------------------------------------------------------

	//the unsigned angle between v1 and v2
	static float Angle(Vector2D v1, Vector2D v2)
	{
		return acos(Dot(v1, v2)); 
	}

	//distance between v1 and v2
	static float Distance(Vector2D v1, Vector2D v2) 
	{ 
		return (v1 - v2).Magnitude(); 
	}

	//returns the sum of the products of v1 and v2
	static float Dot(Vector2D v1, Vector2D v2)
	{
		return v1.x*v2.x + v1.y*v2.y; 
	}

	static float Cross(Vector2D v1, Vector2D v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	//linearly interpolate between v1 and v2
	static Vector2D Lerp(Vector2D v1, Vector2D v2, float alpha) 
	{ 
		return (v1 * alpha) + (v2 * (1 - alpha)); 
	}

	//returns a vector that is the reflection v against the normal
	static Vector2D Reflect(Vector2D v, Vector2D normal)
	{
		Vector2D n = normal.GetNormalized();
		return v - (n*(2.0f * Dot(v, n)));
	}

	// return projection v1 on to v2
	static Vector2D Projection(Vector2D v1, Vector2D v2)
	{
		float v2SqrMagnitude = v2.SqrMagnitude();
		if (v2SqrMagnitude > 1.0e-8f)
			return v2 * (Dot(v2, v1) / v2SqrMagnitude);
		else
			return Vector2D();
	}

	//Operators--------------------------------------------------------------
	Vector2D operator*(float scaler)
	{
		return Vector2D(x * scaler, y *scaler);
	}

	Vector2D operator*(Vector2D other)
	{
		return Vector2D(x * other.x, y * other.y);
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

	Vector2D operator-(void)const
	{
		return Vector2D(-x, -y);
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

	Vector2D operator=(const Vector3D& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
};

inline Vector2D operator*(float scaler, const Vector2D& v)
{
	return Vector2D(scaler * v.x, scaler * v.y);
}

inline Vector2D operator/(float scaler, const Vector2D& v)
{
	return Vector2D(scaler / v.x, scaler / v.y);
}
#endif // !_VECTOR_H