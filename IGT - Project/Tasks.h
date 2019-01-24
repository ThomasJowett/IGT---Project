#pragma once
#include "BrainTree.h"
#include "Attack.h"
#include <iostream> //TODO : remove
#include "Astar.h"

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
		Vector2D goal = blackboard->getVector2D(mBlackboardKey);
		//Get path
		std::cout << "Getting path to" << goal.to_string() << std::endl;

		mPath = Astar::Generator::GetInstance()->FindPath(mControlledPawn->GetTransform()->mPosition, goal);

		for (auto point : mPath)
		{
			std::cout << point.to_string() << std::endl;
		}
	}

	Status update(float deltaTime) override
	{
		//move pawn towards next location in path
		//if(pawn location is within acceptable raduis of goal)
		//return Node::Status::Success;
		//else
		//return Node::Status::Running;

		std::cout << "Moveing" << std::endl;
		return Node::Status::Success;
	}
private:
	float mAcceptableRadius;
	std::string mBlackboardKey;

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