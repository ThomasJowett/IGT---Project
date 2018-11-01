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
	if (mIsAsleep)
	{
		return;
	}

	//cache transform
	Transform* transform = GetParent()->GetTransform();

	AddForce(mVelocity * -mPhysicsMaterial.drag);
	Vector2D acceleration = mNetForce / mMass;

	Vector2D position = Vector2D(transform->mPosition.x, transform->mPosition.y);
	position += (mVelocity*deltaTime) + (acceleration*(deltaTime*deltaTime)) / 2;

	transform->mPosition = Vector3D(position.x, position.y, transform->mPosition.z);

	mVelocity += acceleration * deltaTime;

	mNetForce = Vector2D();

	if (!mFreezeRotation)
	{
		AddTorque(mAngularVelocity * -mPhysicsMaterial.angularDrag);

		float mAngularAcceleration = mNetTorque / mRotationalInertia;

		mAngularVelocity += mAngularAcceleration * deltaTime;

		transform->mRotation += mAngularVelocity;
	}

	mNetTorque = 0.0f;

	if (mVelocity.SqrMagnitude() < 0.5f && mAngularVelocity < 0.001f)
	{
		mIsAsleep = true;
		mVelocity = { 0,0 };
		mAngularVelocity = 0.0f;
	}
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

	mIsAsleep = false;
}

void RigidBody2D::AddPointForce(Vector2D force, Vector2D position)
{
	Transform * parentTransform = GetParent()->GetWorldTransform();
	Vector2D relativePosition = Vector2D(parentTransform->mPosition.x + position.x, parentTransform->mPosition.y + position.y);
	float torque = Vector2D::Cross(Vector2D(force.x, force.y), Vector2D(relativePosition.x, relativePosition.y));
	AddTorque(torque);
	AddForce(force);

	mIsAsleep = false;
}

void RigidBody2D::AddRelativeForce(Vector2D force, Vector2D position)
{
	float torque = Vector2D::Cross(Vector2D(force.x, force.y), Vector2D(position.x, position.y));

	AddTorque(torque);
	AddForce(force);

	mIsAsleep = false;
}

void RigidBody2D::AddTorque(float torque)
{
	mNetTorque += torque;

	mIsAsleep = false;
}

void RigidBody2D::ApplyImpulse(Vector2D impulse)
{
	mVelocity += mInverseMass * impulse;

	mIsAsleep = false;
}

void RigidBody2D::SetVelocity(Vector2D velocity)
{
	mVelocity = velocity;

	if (mVelocity.SqrMagnitude() > 0.0f || mVelocity.SqrMagnitude() < 0.0f)
		mIsAsleep = false;
}
