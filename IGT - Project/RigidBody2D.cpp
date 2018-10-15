#include "RigidBody2D.h"



RigidBody2D::RigidBody2D(GameObject* parent, float mass, Vector2D velocity, PhysicsMaterial physicsMaterial)
	:mMass(mass), mVelocity(velocity), mPhysicsMaterial(physicsMaterial),
	iUpdateable(parent)
{
	mInverseMass = 1 / mass;
	mFreezeRotation = true;
}

RigidBody2D::RigidBody2D(GameObject * parent, float mass, Vector2D velocity, float inertia, float angularVelocity, PhysicsMaterial physicsMaterial)
	:mMass(mass), mVelocity(velocity),mRotationalInertia(inertia), mAngularVelocity(angularVelocity),mPhysicsMaterial(physicsMaterial),
	iUpdateable(parent)
{
	mInverseMass = 1 / mass;
	mFreezeRotation = false;
}


RigidBody2D::~RigidBody2D()
{
}

void RigidBody2D::Update(float deltaTime)
{
	AddForce(mVelocity * -mPhysicsMaterial.drag);
	Vector2D acceleration = mNetForce / mMass;

	Vector2D position = Vector2D(GetParent()->GetTransform()->mPosition.x, GetParent()->GetTransform()->mPosition.y);
	position += (mVelocity*deltaTime) + (acceleration*(deltaTime*deltaTime)) / 2;

	GetParent()->GetTransform()->mPosition = Vector3D(position.x, position.y, GetParent()->GetTransform()->mPosition.z);

	mVelocity += acceleration * deltaTime;

	mNetForce = Vector2D();

	if (!mFreezeRotation)
	{
		AddTorque(mAngularVelocity * -mPhysicsMaterial.angularDrag);

		float mAngularAcceleration = mNetTorque / mRotationalInertia;

		mAngularVelocity += mAngularAcceleration * deltaTime;

		GetParent()->GetTransform()->mRotation += mAngularVelocity;		
	}

	mNetTorque = 0.0f;
}

Component * RigidBody2D::Clone()
{
	RigidBody2D* newRigidBody = new RigidBody2D(nullptr, mMass, mVelocity, mRotationalInertia, mAngularVelocity, mPhysicsMaterial);
	newRigidBody->FreezeRotation(mFreezeRotation);
	return newRigidBody;
}

void RigidBody2D::AddForce(Vector2D force)
{
	mNetForce += force;
}

void RigidBody2D::AddPointForce(Vector2D force, Vector2D position)
{
	Vector2D relativePosition = Vector2D(GetParent()->GetTransform()->mPosition.x + position.x, GetParent()->GetTransform()->mPosition.y + position.y);
	float torque = Vector2D::Cross(Vector2D(force.x, force.y), Vector2D(relativePosition.x, relativePosition.y));
	AddTorque(torque);
	AddForce(force);
}

void RigidBody2D::AddRelativeForce(Vector2D force, Vector2D position)
{
	float torque = Vector2D::Cross(Vector2D(force.x, force.y), Vector2D(position.x, position.y));

	AddTorque(torque);
	AddForce(force);
}

void RigidBody2D::AddTorque(float torque)
{
	mNetTorque += torque;
}

void RigidBody2D::ApplyImpulse(Vector2D impulse)
{
	mVelocity += mInverseMass * impulse;
}
