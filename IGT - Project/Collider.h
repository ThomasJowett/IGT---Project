#pragma once
#include "iComponents.h"
#include "Transform.h"
#include "Messaging.h"

enum ColliderType
{
	BOX2D,
	CIRCLE2D
};

enum class OverlapEvent { BEGIN_OVERLAP, END_OVERLAP};

class Box2D;
class Circle2D;
class GameObject;

class Collider :
	public Component, public Subject<OverlapEvent, GameObject*&>
{
public:
	Collider(Transform* transform, ColliderType type, Vector2D offset)
		: mTransform(transform), mType(type), mOffset(offset) {}
	~Collider() {}
		
	Transform * mTransform;
	virtual bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth) = 0;
	virtual bool ContainsPoint(Vector2D point) = 0;

	Vector2D GetCentre() 
	{
		return Vector2D( mTransform->mPosition.x + mOffset.x, mTransform->mPosition.y + mOffset.y); 
	}

	ColliderType mType;
protected:
	bool mCollided; //TODO: have a list of colliders that this is colliding with
	Vector2D mOffset;

	Vector2D* GetAxis(Vector2D* box1Corners, Vector2D* box2Corners);
	bool TestAxis(Vector2D axis, float minA, float maxA, float minB, float maxB, Vector2D & mtvAxis, float & mtvDistance);
	bool BoxBox(Box2D* box1, Box2D* box2, Vector2D & normal, float& penetrationDepth);
	bool BoxCircle(Box2D* box, Circle2D* circle, Vector2D & normal, float& penetrationDepth);
	bool CircleCircle(Circle2D* circle1, Circle2D* circle2, Vector2D & normal, float& penetrationDepth);
};

class Box2D : public Collider
{
public:
	Box2D(Transform* transform, float width, float height, Vector2D offset)
		: mWidth(width), mHeight(height), Collider(transform, BOX2D, offset) {}
	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth)override;
	bool ContainsPoint(Vector2D point)override;

	Vector2D* GetCorners();
	
private:
	float mWidth;
	float mHeight;
};

class Circle2D : public Collider
{
public:
	Circle2D(Transform* transform, float radius, Vector2D offset)
		: mRadius(radius), Collider(transform, CIRCLE2D, offset) {}
	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth)override;
	bool ContainsPoint(Vector2D point)override;
	float GetRadius() const { return mRadius; }
private:
	float mRadius;
};
