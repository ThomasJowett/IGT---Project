#pragma once
#include "iComponents.h"

class RigidBody2D
	:public iUpdateable
{
public:
	RigidBody2D(GameObject* parent, float mass, Vector2D velocity, PhysicsMaterial physicsMaterial);
	RigidBody2D(GameObject* parent, float mass, Vector2D velocity, float inertia, float angularVelocity, PhysicsMaterial physicsMaterial);
	virtual ~RigidBody2D();

	void Update(float deltaTime)override;
	Component* Clone()override;

	void AddForce(Vector2D force);
	void AddPointForce(Vector2D force, Vector2D position);
	void AddRelativeForce(Vector2D force, Vector2D position);
	void AddTorque(float torque);
	void ApplyImpulse(Vector2D impulse);

	//Getters and Setters
	Vector2D GetVelocity()const { return mVelocity; }
	void SetVelocity(Vector2D velocity) { mVelocity = velocity; }

	float GetMass()const { return mMass; }
	float GetInverseMass()const { return mInverseMass; }
	void SetMass(float mass) { mMass = mass; mInverseMass = 1 / mass; }

	bool IsRotationFrozen() const { return mFreezeRotation; }
	void FreezeRotation(bool freeze) { mFreezeRotation = freeze; }

	float GetAngularVelocity()const { return mAngularVelocity; }
	void SetAngularVelocity(float velocity) { mAngularVelocity = velocity; }

	float GetInertia()const { return mRotationalInertia; }
	void SetInertia(float inertia) { mRotationalInertia = inertia; }

	PhysicsMaterial GetPhysicsMaterial()const { return mPhysicsMaterial; }
	void SetPhysicsMaterial(PhysicsMaterial physicsMaterial) { mPhysicsMaterial = physicsMaterial; }

private:
	float mMass;
	float mInverseMass;
	Vector2D mVelocity;
	Vector2D mNetForce;

	bool mFreezeRotation;

	float mRotationalInertia;
	float mAngularVelocity;
	float mNetTorque;

	PhysicsMaterial mPhysicsMaterial;
};