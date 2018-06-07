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
	Collider(GameObject* parent, ColliderType type, Vector2D offset)
		: mType(type), mOffset(offset), Component(parent) {}
	~Collider() {}

	virtual Component* Clone() override = 0;
		
	virtual bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth) = 0;
	virtual bool ContainsPoint(Vector2D point) = 0;

	Vector2D GetCentre() 
	{
		Matrix4x4 translate = Matrix4x4::Translate(GetParent()->GetTransform()->mPosition);
		Matrix4x4 rotation = Matrix4x4::RotateZ(GetParent()->GetTransform()->mRotation);
		Matrix4x4 offset = Matrix4x4::Translate(Vector3D(mOffset.x, mOffset.y, 0));
		return (translate * rotation * offset).ToVector2D();
	}

	ColliderType mType;
protected:
	bool mCollided; //TODO: have a list of colliders that this is colliding with
	Vector2D mOffset;

	std::vector<Vector2D> GetAxis(std::vector<Vector2D> box1Corners, std::vector<Vector2D> box2Corners);
	void ProjectCornersOnAxis(Vector2D axis, std::vector<Vector2D> corners, float & min, float & max);
	bool TestAxis(Vector2D axis, float minA, float maxA, float minB, float maxB, Vector2D & mtvAxis, float & mtvDistance);
	bool BoxBox(Box2D* box1, Box2D* box2, Vector2D & normal, float& penetrationDepth);
	bool BoxCircle(Box2D* box, Circle2D* circle, Vector2D & normal, float& penetrationDepth);
	bool CircleCircle(Circle2D* circle1, Circle2D* circle2, Vector2D & normal, float& penetrationDepth);
};

class Box2D : public Collider
{
public:
	Box2D(GameObject* parent, float width, float height, Vector2D offset)
		: mWidth(width), mHeight(height), Collider(parent, BOX2D, offset) {}
	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth)override;
	bool ContainsPoint(Vector2D point)override;

	std::vector<Vector2D> GetCorners();
	
	Component* Clone()override;
private:
	float mWidth;
	float mHeight;
};

class Circle2D : public Collider
{
public:
	Circle2D(GameObject* parent, float radius, Vector2D offset)
		: mRadius(radius), Collider(parent, CIRCLE2D, offset) {}
	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth)override;
	bool ContainsPoint(Vector2D point)override;
	float GetRadius() const { return mRadius; }

	Component* Clone()override;
private:
	float mRadius;
};
