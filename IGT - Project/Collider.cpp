#include "Collider.h"
#include <iostream>

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
	std::vector<Vector2D> box1Corners = box1->GetCorners();
	std::vector<Vector2D> box2Corners = box2->GetCorners();
	std::vector<Vector2D> axis = GetAxis(box1Corners, box2Corners);

	float mtvDistance = FLT_MAX;
	Vector2D mtvAxis;

	for (int i = 0; i < 4; i++)
	{
		float minA, maxA, minB, maxB;
		ProjectCornersOnAxis(axis[i], box1Corners, minA, maxA);
		ProjectCornersOnAxis(axis[i], box2Corners, minB, maxB);

		if (!TestAxis(axis[0], minA, maxA, minB, maxB, mtvAxis, mtvDistance))
			return false;
	}
	normal = mtvAxis.GetNormalized();
	penetrationDepth = (float)sqrt(mtvDistance);
	return true;
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
	std::vector<Vector2D> corners = GetCorners();
	Vector2D axis1 = (corners[1] - corners[0]).GetNormalized();
	Vector2D axis2 = (corners[3] - corners[0]).GetNormalized();

	float min, max;
	ProjectCornersOnAxis(axis1, corners, min, max);
	float pointOnAxis = Vector2D::Dot(point, axis1);
	
	if(pointOnAxis < min || pointOnAxis > max)
		return false;

	pointOnAxis = Vector2D::Dot(point, axis2);
	ProjectCornersOnAxis(axis2, corners, min, max);

	if (pointOnAxis < min || pointOnAxis > max)
		return false;
	
	return true;
}

std::vector<Vector2D> Box2D::GetCorners()
{
	float halfWidth = mWidth / 2;
	float halfHeight = mHeight / 2;

	std::vector<Vector2D> corners;
	Matrix4x4 translateWorld = Matrix4x4::Translate(mTransform->mPosition);
	Matrix4x4 rotation = Matrix4x4::RotateZ(mTransform->mRotation);
	Matrix4x4 offset = Matrix4x4::Translate(Vector3D(mOffset.x, mOffset.y, 0));

	Matrix4x4 translateCorner = Matrix4x4::Translate(Vector3D(-halfWidth, -halfHeight, 0));
	Matrix4x4 mPosition = translateWorld  * rotation * offset * translateCorner;
	corners.push_back((translateWorld*rotation*offset*translateCorner).ToVector2D());

	translateCorner = Matrix4x4::Translate(Vector3D(halfWidth, -halfHeight, 0));
	corners.push_back((translateWorld*rotation*offset*translateCorner).ToVector2D());

	translateCorner = Matrix4x4::Translate(Vector3D(halfWidth, halfHeight, 0));
	corners.push_back((translateWorld*rotation*offset*translateCorner).ToVector2D());

	translateCorner = Matrix4x4::Translate(Vector3D(-halfWidth, halfHeight, 0));
	corners.push_back((translateWorld*rotation*offset*translateCorner).ToVector2D());

	return corners;
}

std::vector<Vector2D> Collider::GetAxis(std::vector<Vector2D> box1Corners, std::vector<Vector2D> box2Corners)
{
	std::vector<Vector2D> axis;
	axis.push_back((box1Corners[1] - box1Corners[0]).GetNormalized());
	axis.push_back((box1Corners[3] - box1Corners[0]).GetNormalized());
	axis.push_back((box2Corners[1] - box1Corners[0]).GetNormalized());
	axis.push_back((box2Corners[3] - box1Corners[0]).GetNormalized());
	return axis;
}

void Collider::ProjectCornersOnAxis(Vector2D axis, std::vector<Vector2D> corners, float & min, float & max)
{
	float dotProduct = Vector2D::Dot(axis, corners[0]);
	min = dotProduct;
	max = dotProduct;

	for (int i = 1; i < corners.size(); i++)
	{
		dotProduct = Vector2D::Dot(axis, corners[i]);
		if (dotProduct < min)
		{
			min = dotProduct;
		}
		else if (dotProduct > max)
		{
			max = dotProduct;
		}
	}
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
