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
		//mGoal = blackboard->getVector2D(mBlackboardKey);
		//Get path
		//std::cout << "Getting path from " << mControlledPawn->GetTransform()->mPosition.to_string() << " to " << mGoal.to_string() << std::endl;

		//mPath = Astar::Generator::GetInstance()->FindPath(mControlledPawn->GetTransform()->mPosition, mGoal);
		//mCurrentWaypoint = mPath.size() - 1;
	}

	Status update(float deltaTime) override
	{
		//Has the goal moved
		if (Vector2D::Distance(mGoal, blackboard->getVector2D(mBlackboardKey)) > mAcceptableRadius)
		{
			mGoal = blackboard->getVector2D(mBlackboardKey);
			mPath = Astar::Generator::GetInstance()->FindPath(mControlledPawn->GetTransform()->mPosition, mGoal);
			mCurrentWaypoint = mPath.size() - 1;

			//return so that it does not move on the same update as the finding path takes place
			return Node::Status::Running;
		}

		//Has pawn reached goal
		if (Vector2D::Distance(mControlledPawn->GetWorldTransform()->mPosition, mGoal) < mAcceptableRadius || mPath.size() == 0)
		{
			return Node::Status::Success;
		}

		if (Vector2D::Distance(mPath[mCurrentWaypoint], mControlledPawn->GetWorldTransform()->mPosition) < mAcceptableRadius)
		{
			//std::cout << "waypoint reached\n";
			mCurrentWaypoint--;
			if (mCurrentWaypoint < 0)
			{
				mCurrentWaypoint = 0;
			}
		}
		else if(Vector2D::Distance(mPath[mCurrentWaypoint], mControlledPawn->GetWorldTransform()->mPosition) < mAcceptableRadius)
		{

		}

		Vector2D force = (mPath[mCurrentWaypoint] - (Vector2D(mControlledPawn->GetWorldTransform()->mPosition)));
		force.Normalize();
		force = force * 1000.0f;
		mControlledPawn->GetComponent<RigidBody2D>()->AddForce(force);

		return Node::Status::Running;
	}
private:
	float mAcceptableRadius;
	std::string mBlackboardKey;

	Vector2D mGoal;

	GameObject* mControlledPawn;
	std::vector<Vector2D> mPath;
	int mCurrentWaypoint;
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