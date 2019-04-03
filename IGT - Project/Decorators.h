#pragma once
#include "BrainTree.h"

class BlackboardBool : public BrainTree::Decorator
{
public:
	BlackboardBool(BrainTree::Blackboard::Ptr blackboard, std::string blackboardkey, bool isSet)
		:mBlackboardKey(blackboardkey), mIsSet(isSet),
		Decorator(blackboard) {}

	Status update(float deltaTime) override
	{
		if (!(blackboard->getBool(mBlackboardKey) ^ mIsSet))
			return child->tick(deltaTime);

		return Status::Failure;
	}
private:
	std::string mBlackboardKey;
	bool mIsSet;
};

class BlackboardCompare : public BrainTree::Decorator
{
public:
	BlackboardCompare(BrainTree::Blackboard::Ptr blackboard, std::string blackboardkey_1, std::string blackboardkey_2, bool isEqual)
		:mBBKey_1(blackboardkey_1), mBBKey_2(blackboardkey_2), mIsEqual(isEqual),
		Decorator(blackboard) {}

	Status update(float deltaTime) override
	{
		if (!((blackboard->getBool(mBBKey_1) == blackboard->getBool(mBBKey_1)) ^ mIsEqual))
			return child->tick(deltaTime);

		return Status::Failure;
	}
private:
	std::string mBBKey_1;
	std::string mBBKey_2;
	bool mIsEqual;
};