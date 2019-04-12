#pragma once
#include "iComponents.h"
#include "Transform.h"
#include "Messaging.h"

enum ColliderType
{
	BOX2D,
	CIRCLE2D,
};

enum CollisionChannel
{
	WORLD_STATIC,
	WORLD_DYNAMIC,
	PLAYER,
	ENEMY,
	PROJECTILE
};

enum class OverlapEvent { BEGIN_OVERLAP, END_OVERLAP};

class Box2D;
class Circle2D;
class GameObject;

class Collider :
	public Component, public Subject<OverlapEvent, Collider*>
{
public:
	Collider(GameObject* parent, ColliderType type, Vector2D offset)
		: mType(type), Component(parent) 
	{
		mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0));
		mCollisionTestedWith = std::vector<Collider*>();
		mChannel = CollisionChannel::WORLD_DYNAMIC;
	}
	Collider(GameObject* parent, ColliderType type, Vector2D offset, bool isTrigger)
		: mType(type), mIsTrigger(isTrigger), Component(parent)
	{
		mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0));
		mCollisionTestedWith = std::vector<Collider*>();
		mChannel = CollisionChannel::WORLD_DYNAMIC;
	}
	Collider(GameObject* parent, ColliderType type, Matrix4x4 offset)
		: mType(type), mOffset(offset), Component(parent) 
	{
		mCollisionTestedWith = std::vector<Collider*>();
		mChannel = CollisionChannel::WORLD_DYNAMIC;
	}
	Collider(GameObject* parent, ColliderType type, Matrix4x4 offset, bool isTrigger, bool generateOverlapEvents, CollisionChannel collisionChannel)
		: mType(type), mOffset(offset), mIsTrigger(isTrigger),mGenerateOverlapEvents(generateOverlapEvents), mChannel(collisionChannel), Component(parent)
	{
		mCollisionTestedWith = std::vector<Collider*>();
	}
	virtual ~Collider() {}

	virtual Component* Clone() override = 0;
		
	virtual bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth) = 0;
	virtual bool ContainsPoint(Vector2D point) = 0;
	virtual bool TestAxis(Vector2D axis, float offset) = 0;
	virtual void GetBounds(float& Xmax, float &Xmin, float &Ymax, float &Ymin)const = 0;
	

	Vector2D GetCentre() const;


	void SetOffset(Vector2D offset) { mOffset = Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0)); }

	ColliderType mType;

	void AddTestedCollisionWith(Collider* collider) { mCollisionTestedWith.push_back(collider); }
	void ClearTestedCollisionWith() { mCollisionTestedWith.clear(); }
	bool HasTestedCollisionWith(Collider* collider);

	bool IsTrigger() const { return mIsTrigger; }
	void SetIsTrigger(bool isTrigger) { mIsTrigger = isTrigger; }

	bool GeneratesOverlapEvents() { return mGenerateOverlapEvents; }

	CollisionChannel GetCollisionChannel() { return mChannel; }

protected:
	std::vector<Collider*> mCollisionTestedWith;
	bool mCollided; //TODO: have a list of colliders that this is colliding with
	Matrix4x4 mOffset;

	bool mIsTrigger = false;
	bool mGenerateOverlapEvents = true;

	CollisionChannel mChannel;

	std::vector<Vector2D> GetAxis(std::vector<Vector2D> box1Corners, std::vector<Vector2D> box2Corners);
	void ProjectCornersOnAxis(Vector2D axis, std::vector<Vector2D> corners, float & min, float & max) const;
	void ProjectCircleOnAxis(Vector2D axis, Circle2D circle, float & min, float & max) const;
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

	Box2D(GameObject* parent, float width, float height, Vector2D offset, bool isTrigger)
		: mWidth(width), mHeight(height), Collider(parent, BOX2D, offset, isTrigger) {}

	Box2D(GameObject* parent, float width, float height, Vector2D offset, bool isTrigger, bool generateOverlapEvents)
		: mWidth(width), mHeight(height), Collider(parent, BOX2D, Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0)), isTrigger, generateOverlapEvents, CollisionChannel::WORLD_DYNAMIC) {}

	Box2D(GameObject* parent, float width, float height, Vector2D offset, bool isTrigger, bool generateOverlapEvents, CollisionChannel collisionChannel)
		: mWidth(width), mHeight(height), Collider(parent, BOX2D, Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0)), isTrigger, generateOverlapEvents, collisionChannel) {}

	Box2D(GameObject* parent, float width, float height, Matrix4x4 offset)
		: mWidth(width), mHeight(height), Collider(parent, BOX2D, offset) {}

	Box2D(GameObject* parent, float width, float height, Matrix4x4 offset, bool isTrigger, bool generateOverlapEvents, CollisionChannel collisionChannel)
		: mWidth(width), mHeight(height), Collider(parent, BOX2D, offset, isTrigger, generateOverlapEvents, collisionChannel) {}

	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth)override;
	bool ContainsPoint(Vector2D point)override;
	bool TestAxis(Vector2D axis, float offset)override;
	void GetBounds(float& Xmax, float &Xmin, float &Ymax, float &Ymin)const override;

	std::vector<Vector2D> GetCorners() const;
	
	Component* Clone()override;

	float GetWidth() { return mWidth; }
	float GetHeight() { return mHeight; }
private:
	float mWidth;
	float mHeight;
};

class Circle2D : public Collider
{
public:
	Circle2D(GameObject* parent, float radius, Vector2D offset)
		: mRadius(radius), Collider(parent, CIRCLE2D, offset) {}

	Circle2D(GameObject* parent, float radius, Matrix4x4 offset)
		: mRadius(radius), Collider(parent, CIRCLE2D, offset) {}

	Circle2D(GameObject* parent, float radius, Vector2D offset, bool isTrigger)
		: mRadius(radius), Collider(parent, CIRCLE2D, offset, isTrigger) {}

	Circle2D(GameObject* parent, float radius, Vector2D offset, bool isTrigger, bool generateOverlapEvents)
		: mRadius(radius), Collider(parent, CIRCLE2D, Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0)), isTrigger, generateOverlapEvents, CollisionChannel::WORLD_DYNAMIC) {}

	Circle2D(GameObject* parent, float radius, Vector2D offset, bool isTrigger, bool generateOverlapEvents, CollisionChannel collisionChannel)
		: mRadius(radius), Collider(parent, CIRCLE2D, Matrix4x4::Translate(Vector3D(offset.x, offset.y, 0)), isTrigger, generateOverlapEvents, collisionChannel) {}

	Circle2D(GameObject* parent, float radius, Matrix4x4 offset, bool isTrigger, bool generateOverlapEvents, CollisionChannel collisionChannel)
		: mRadius(radius), Collider(parent, CIRCLE2D, offset, isTrigger, generateOverlapEvents, collisionChannel) {}

	bool IntersectsCollider(Collider* otherCollider, Vector2D& normal, float& penetrationDepth)override;
	bool ContainsPoint(Vector2D point)override;
	bool TestAxis(Vector2D axis, float offset)override;
	void GetBounds(float& Xmax, float &Xmin, float &Ymax, float &Ymin)const override;
	float GetRadius() const { return mRadius; }

	Component* Clone()override;

private:
	float mRadius;
};