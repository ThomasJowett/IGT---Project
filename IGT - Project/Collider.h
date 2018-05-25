#pragma once
#include "iComponents.h"
#include "Transform.h"

enum ColliderType
{
	BOX2D,
	CIRCLE2D
};

class Collider :
	public Component
{
public:
	Collider(Transform* transform, ColliderType type, Vector2D offset)
		: mTransform(transform), mType(type), mOffset(offset) {}
	~Collider() {}
		
	Transform * mTransform;
	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth);
	bool ContatinsPoint(Vector2D point);

	Vector2D GetCentre() 
	{
		return Vector2D( mTransform->mPosition.x + mOffset.x, mTransform->mPosition.y + mOffset.y); 
	}
	
protected:
	ColliderType mType;
	Vector2D mOffset;
};

class Box2D : public Collider
{
public:
	Box2D(Transform* transform, float width, float height, Vector2D offset)
		: mWidth(width), mHeight(height), Collider(transform, BOX2D, offset) {}
private:
	float mWidth;
	float mHeight;
};

class Circle2D : public Collider
{
public:
	Circle2D(Transform* transform, float radius, Vector2D offset)
		: mRadius(radius), Collider(transform, CIRCLE2D, offset) {}
private:
	float mRadius;
};
