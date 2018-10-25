#include "Collider.h"
#include <iostream>

//takes a min and max value for two colliders along an axis and returns if the ranges are overlapping
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

		if (!TestAxis(axis[i], minA, maxA, minB, maxB, mtvAxis, mtvDistance))
			return false;
	}
	normal = mtvAxis.GetNormalized();
	penetrationDepth = (float)sqrt(mtvDistance);
	
	return true;
}

bool Collider::BoxCircle(Box2D* box, Circle2D* circle, Vector2D & normal, float & penetrationDepth)
{
	float radius = circle->GetRadius();
	
	std::vector<Vector2D> boxCorners = box->GetCorners();

	Vector2D axis1 = (boxCorners[1] - boxCorners[0]).GetNormalized();
	Vector2D axis2 = (boxCorners[3] - boxCorners[0]).GetNormalized();

	float minA, maxA, minB, maxB;
	float mtvDistance = FLT_MAX;
	Vector2D mtvAxis;

	ProjectCornersOnAxis(axis1, boxCorners, minA, maxA);
	ProjectCircleOnAxis(axis1, *circle, minB, maxB);
	if (!TestAxis(axis1, minA, maxA, minB, maxB, mtvAxis, mtvDistance))
		return false;

	ProjectCornersOnAxis(axis2, boxCorners, minA, maxA);
	ProjectCircleOnAxis(axis2, *circle, minB, maxB);
	if (!TestAxis(axis2, minA, maxA, minB, maxB, mtvAxis, mtvDistance))
		return false;
	
	Vector2D seperation = (circle->GetCentre() - box->GetCentre()).GetNormalized();
	ProjectCornersOnAxis(seperation, boxCorners, minA, maxA);
	ProjectCircleOnAxis(seperation, *circle, minB, maxB);
	
	if (!TestAxis(seperation, minA, maxA, minB, maxB, mtvAxis, mtvDistance))
		return false;

	normal = mtvAxis.GetNormalized();
	penetrationDepth = (float)sqrt(mtvDistance);

	return true;
}

bool Collider::CircleCircle(Circle2D* circle1, Circle2D* circle2, Vector2D & normal, float & penetrationDepth)
{
	Vector2D seperation = circle2->GetCentre() - circle1->GetCentre();

	float sumOfBoundingRadii = circle1->GetRadius() + circle2->GetRadius();

	if ((sumOfBoundingRadii*sumOfBoundingRadii) > seperation.SqrMagnitude())
	{
		penetrationDepth = seperation.Magnitude() - sumOfBoundingRadii;
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


bool Box2D::TestAxis(Vector2D axis, float offset)
{
	std::vector<Vector2D> corners = GetCorners();
	float min, max;
	ProjectCornersOnAxis(axis, corners, min, max);

	return min > offset;
}

void Box2D::GetBounds(float & Xmax, float & Xmin, float & Ymax, float & Ymin)
{
	std::vector<Vector2D> corners = GetCorners();

	ProjectCornersOnAxis(Vector2D(1, 0), corners, Xmin, Xmax);
	ProjectCornersOnAxis(Vector2D(0, 1), corners, Ymin, Ymax);
}

std::vector<Vector2D> Box2D::GetCorners()
{
	float halfWidth = mWidth / 2;
	float halfHeight = mHeight / 2;

	std::vector<Vector2D> corners;
	Matrix4x4 translateWorld = Matrix4x4::Translate(GetParent()->GetWorldTransform()->mPosition);
	Matrix4x4 rotation = Matrix4x4::RotateZ(GetParent()->GetWorldTransform()->mRotation);

	Matrix4x4 translateCorner = Matrix4x4::Translate(Vector3D(-halfWidth, -halfHeight, 0));
	Matrix4x4 mPosition = translateWorld  * rotation * mOffset * translateCorner;
	corners.push_back((translateWorld*rotation*mOffset*translateCorner).ToVector2D());

	translateCorner = Matrix4x4::Translate(Vector3D(halfWidth, -halfHeight, 0));
	corners.push_back((translateWorld*rotation*mOffset*translateCorner).ToVector2D());

	translateCorner = Matrix4x4::Translate(Vector3D(halfWidth, halfHeight, 0));
	corners.push_back((translateWorld*rotation*mOffset*translateCorner).ToVector2D());

	translateCorner = Matrix4x4::Translate(Vector3D(-halfWidth, halfHeight, 0));
	corners.push_back((translateWorld*rotation*mOffset*translateCorner).ToVector2D());

	return corners;
}

Component * Box2D::Clone()
{
	return new Box2D(nullptr, mWidth, mHeight, mOffset);
}

std::vector<Vector2D> Collider::GetAxis(std::vector<Vector2D> box1Corners, std::vector<Vector2D> box2Corners)
{
	std::vector<Vector2D> axis;
	axis.push_back((box1Corners[1] - box1Corners[0]).GetNormalized());
	axis.push_back((box1Corners[3] - box1Corners[0]).GetNormalized());
	axis.push_back((box2Corners[1] - box2Corners[0]).GetNormalized());
	axis.push_back((box2Corners[3] - box2Corners[0]).GetNormalized());
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

void Collider::ProjectCircleOnAxis(Vector2D axis, Circle2D circle, float & min, float & max)
{
	float dotProduct = Vector2D::Dot(axis, circle.GetCentre());

	min = dotProduct - circle.GetRadius();
	max = dotProduct + circle.GetRadius();
}

bool Circle2D::IntersectsCollider(Collider * otherCollider, Vector2D & normal, float & penetrationDepth)
{
	if (otherCollider->mType == BOX2D)
	{
		Box2D * otherBox = dynamic_cast<Box2D*>(otherCollider);
		if (BoxCircle(otherBox, this, normal, penetrationDepth))
		{
			//because BoxCircle() tests from box to circle and we need circle to box the normal is flipped
			normal = -normal;
			penetrationDepth /= 100;//TODO : fix this properly
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

bool Circle2D::TestAxis(Vector2D axis, float offset)
{
	float min, max;
	ProjectCircleOnAxis(axis.Perpendicular(), *this, min, max);

	return min > offset;
}

void Circle2D::GetBounds(float & Xmax, float & Xmin, float & Ymax, float & Ymin)
{
	Vector2D position = GetCentre();

	Xmax = position.x + mRadius;
	Xmin = position.x - mRadius;

	Ymax = position.y + mRadius;
	Ymin = position.y - mRadius;
}

Component * Circle2D::Clone()
{
	return new Circle2D(nullptr, mRadius, mOffset);
}