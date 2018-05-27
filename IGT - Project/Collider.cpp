#include "Collider.h"

bool Collider::TestAxis(Vector2D axis, float minA, float maxA, float minB, float maxB, Vector2D & mtvAxis, float & mtvDistance)
{
	if (axis.SqrMagnitude() < 1.0e-8f)
		return true;

	float d0 = (maxB - minA);
	float d1 = (maxA - minB);

	if (d0 <= 0.0f || d1 <= 0.0f)
		return false;

	float overlap = (d0 < d1) ? d0 : -d1;

	Vector2D seperation = axis * (overlap / axis.SqrMagnitude());

	if (seperation.SqrMagnitude() < mtvDistance)
	{
		mtvDistance = seperation.SqrMagnitude();
		mtvAxis = seperation;
	}
	return true;
}

bool Collider::BoxBox(Box2D* box1, Box2D* box2, Vector2D & normal, float & penetrationDepth)
{
	GetAxis(box1->GetCorners(), box2->GetCorners());
	return false;
}

bool Collider::BoxCircle(Box2D* box, Circle2D* circle, Vector2D & normal, float & penetrationDepth)
{
	Vector2D seperation = circle->GetCentre() - box->GetCentre();
	Vector2D closestPoint;

	return false;
}

bool Collider::CircleCircle(Circle2D* circle1, Circle2D* circle2, Vector2D & normal, float & penetrationDepth)
{
	Vector2D seperation = circle2->GetCentre() - circle1->GetCentre();

	float sumOfBoundingRadii = circle1->GetRadius() + circle2->GetRadius();

	if ((sumOfBoundingRadii*sumOfBoundingRadii) > seperation.SqrMagnitude())
	{
		penetrationDepth = sumOfBoundingRadii - seperation.Magnitude();
		normal = seperation.GetNormalized();
		return true;
	}

	return false;
}

bool Box2D::IntersectsCollider(Collider * otherCollider, Vector2D & normal, float & penetrationDepth)
{
	if (otherCollider->mType == BOX2D)
	{
		Box2D * otherBox = dynamic_cast<Box2D*>(otherCollider);
		return BoxBox(this, otherBox, normal, penetrationDepth);
	}
	else if(otherCollider->mType == CIRCLE2D)
	{
		Circle2D * otherCircle = dynamic_cast<Circle2D*>(otherCollider);
		return BoxCircle(this, otherCircle, normal, penetrationDepth);
	}

	return false;
}

bool Box2D::ContainsPoint(Vector2D point)
{
	return false;
}

Vector2D* Box2D::GetCorners()
{
	float halfWidth = mWidth / 2;
	float halfHeight = mHeight / 2;

	Vector2D corners[4];
	corners[0] = Vector2D(mTransform->mPosition.x - halfWidth, mTransform->mPosition.y - halfHeight) + mOffset;
	corners[1] = Vector2D(mTransform->mPosition.x + halfWidth, mTransform->mPosition.y - halfHeight) + mOffset;
	corners[2] = Vector2D(mTransform->mPosition.x + halfWidth, mTransform->mPosition.y + halfHeight) + mOffset;
	corners[3] = Vector2D(mTransform->mPosition.x - halfWidth, mTransform->mPosition.y + halfHeight) + mOffset;

	corners[0] = Matrix4x4::MulVec2(Matrix4x4::RotateZ(mTransform->mRotation), corners[0]);
	corners[1] = Matrix4x4::MulVec2(Matrix4x4::RotateZ(mTransform->mRotation), corners[1]);
	corners[2] = Matrix4x4::MulVec2(Matrix4x4::RotateZ(mTransform->mRotation), corners[2]);
	corners[3] = Matrix4x4::MulVec2(Matrix4x4::RotateZ(mTransform->mRotation), corners[3]);

	return corners;
}

Vector2D * Collider::GetAxis(Vector2D * box1Corners, Vector2D * box2Corners)
{
	Vector2D axis[4];
	axis[0] = (box1Corners[1] - box1Corners[0]).GetNormalized();
	axis[1] = (box1Corners[1] - box1Corners[2]).GetNormalized();
	axis[2] = (box2Corners[1] - box1Corners[0]).GetNormalized();
	axis[3] = (box2Corners[1] - box1Corners[2]).GetNormalized();
	return nullptr;
}

bool Circle2D::IntersectsCollider(Collider * otherCollider, Vector2D & normal, float & penetrationDepth)
{
	if (otherCollider->mType == BOX2D)
	{
		Box2D * otherBox = dynamic_cast<Box2D*>(otherCollider);
		if (BoxCircle(otherBox, this, normal, penetrationDepth))
		{
			normal * -1.0f;
			return true;
		}
		else
			return false;
	}
	else if(otherCollider->mType == CIRCLE2D)
	{
		Circle2D * otherCircle = dynamic_cast<Circle2D*>(otherCollider);
		return CircleCircle(this, otherCircle, normal, penetrationDepth);
	}

	return false;
}

bool Circle2D::ContainsPoint(Vector2D point)
{
	Vector2D distance = point - GetCentre();
	return (distance.SqrMagnitude() > mRadius * mRadius);
}
