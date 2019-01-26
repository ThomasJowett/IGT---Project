#pragma once
#include "BrainTree.h"
#include "Attack.h"
#include <iostream> //TODO : remove
#include "Astar.h"
#include "RigidBody2D.h"

//Move AI Pawn to a location
class MoveTo : public BrainTree::Leaf
{
public:
	MoveTo(BrainTree::Blackboard::Ptr blackboard, float acceptableRadius, std::string blackboardKey, GameObject* controlledPawn)
		:mAcceptableRadius(acceptableRadius), mBlackboardKey(blackboardKey), mControlledPawn(controlledPawn),
		Leaf(blackboard)
	{
	}
	void initialize()
	{
		mGoal = blackboard->getVector2D(mBlackboardKey);
		//Get path
		std::cout << "Getting path from " << mControlledPawn->GetTransform()->mPosition.to_string() << " to " << mGoal.to_string() << std::endl;

		mPath = Astar::Generator::GetInstance()->FindPath(mControlledPawn->GetTransform()->mPosition, mGoal);

		for (auto point : mPath)
		{
			std::cout << point.to_string() << std::endl;
		}
	}

	Status update(float deltaTime) override
	{
		if (Vector2D::Distance(mControlledPawn->GetWorldTransform()->mPosition, mGoal) < mAcceptableRadius)
		{
			return Node::Status::Success;
		}

		if (Vector2D::Distance(mGoal, blackboard->getVector2D(mBlackboardKey)) > mAcceptableRadius)
		{
			mGoal = blackboard->getVector2D(mBlackboardKey);
			mPath = Astar::Generator::GetInstance()->FindPath(mControlledPawn->GetTransform()->mPosition, mGoal);
		}
		if (Vector2D::Distance(mPath.back(), mControlledPawn->GetWorldTransform()->mPosition) < mAcceptableRadius)
		{
			std::cout << "waypoint reached\n";
			mPath.pop_back();
			if(mPath.size() == 0)
				return Node::Status::Success;
		}

		Vector2D force = (mPath.back() - (Vector2D(mControlledPawn->GetWorldTransform()->mPosition)));
		force.Normalize();
		force = force * 1000.0f;
		mControlledPawn->GetComponent<RigidBody2D>()->AddForce(force);

		//Vector3D Position = mControlledPawn->GetTransform()->mPosition;
		//
		//Position.x = mPath.back().x;
		//Position.y = mPath.back().y;
		//
		//mControlledPawn->GetTransform()->mPosition = Position;

		//std::cout << force.to_string() << std::endl;

		

		
		//move pawn towards next location in path
		//if(pawn location is within acceptable raduis of goal)
		//return Node::Status::Success;
		//else
		//return Node::Status::Running;

		//std::cout << "Moveing" << std::endl;
		return Node::Status::Running;
	}
private:
	float mAcceptableRadius;
	std::string mBlackboardKey;

	Vector2D mGoal;

	GameObject* mControlledPawn;
	std::vector<Vector2D> mPath;
};

//Wait for the specified time when executed
class Wait : public BrainTree::Leaf
{
public:
	Wait(float waitTime)
		:mWaitTime(waitTime)
	{
		mCurrentTime = 0.0f;
	}

	void initialize()
	{
		mCurrentTime = mWaitTime;
	}

	Status update(float deltaTime) override
	{
		mCurrentTime -= deltaTime;

		if (mCurrentTime <= 0.0f)
		{
			mCurrentTime = 0.0f;
			return Node::Status::Success;
		}
		else
			return Node::Status::Running;
	}

private:
	float mWaitTime;
	float mCurrentTime;
};

class AttackTask : public BrainTree::Leaf
{
public:
	AttackTask(GameObject* pawn)
	{
		mPawn = pawn->GetComponent<Attack>();
	}

	void initialize()
	{
		mPawn->BeginAttack();
	}

	Status update(float deltaTime)
	{
		return Node::Status::Success;
	}

private:
	Attack* mPawn;
};