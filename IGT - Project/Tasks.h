#pragma once
#include "BrainTree.h"
#include "Attack.h"
#include "Astar.h"
#include "RigidBody2D.h"
#include "Messaging.h"

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
	}

	Status update(float deltaTime) override
	{
		if (blackboard->getVector2D(mBlackboardKey) == Vector2D())
		{
			return Node::Status::Failure;
		}

		Vector2D pawnPosition = mControlledPawn->GetWorldTransform().mPosition;
		//Has the goal moved
		if (Vector2D::Distance(mGoal, blackboard->getVector2D(mBlackboardKey)) > mAcceptableRadius)
		{
			mGoal = blackboard->getVector2D(mBlackboardKey);
			mPath = Astar::Generator::GetInstance()->FindPath(pawnPosition, mGoal);
			mCurrentWaypoint = mPath.size() - 1;

			//return so that it does not move on the same update as the finding path takes place
			return Node::Status::Running;
		}

		//Has pawn reached goal
		if (Vector2D::Distance(pawnPosition, mGoal) < mAcceptableRadius || mPath.size() == 0)
		{
			return Node::Status::Success;
		}

		if (Vector2D::Distance(mPath[mCurrentWaypoint], pawnPosition) < mAcceptableRadius)
		{
			mCurrentWaypoint--;
			if (mCurrentWaypoint < 0)
			{
				mCurrentWaypoint = 0;
			}
		}

		//has the pawn been pushed off it's path
		else if(Vector2D::Distance(mPath[mCurrentWaypoint], pawnPosition) > mAcceptableRadius * 10.0f)
		{
			mPath = Astar::Generator::GetInstance()->FindPath(pawnPosition, mGoal);
			mCurrentWaypoint = mPath.size() - 1;

			//return so that it does not move on the same update as the finding path takes place
			return Node::Status::Running;
		}

		Vector2D force = (mPath[mCurrentWaypoint] - (Vector2D(pawnPosition)));
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
			std::cout << "end of wait\n";
			return Node::Status::Success;
		}
		else
		{
			std::cout << mCurrentTime <<"\n";
			return Node::Status::Running;
		}
	}

private:
	float mWaitTime;
	float mCurrentTime;
};

class AttackTask : public BrainTree::Leaf, public Observer<AttackEvent, int>
{
public:
	AttackTask(GameObject* pawn)
	{
		mPawn = pawn->GetComponent<Attack>();
	}

	void initialize()
	{
		
	}

	Status update(float deltaTime)
	{
		return (mPawn->BeginAttack()) ? Node::Status::Success : Node::Status::Running;
	}

	void OnNotify(AttackEvent event, int value)override
	{
		switch (event)
		{
		case AttackEvent::ON_ATTACK_END:

			break;
		default:
			break;
		}
	}

private:
	Attack* mPawn;
};