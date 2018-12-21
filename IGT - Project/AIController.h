#pragma once

#include "BrainTree.h"
#include "iComponents.h"
#include "Tasks.h"
#include "Attack.h"
#include "Decorators.h"

class AIController
	:public iUpdateable
{
public:
	AIController(GameObject* parent);
	~AIController() {};

	void Update(float deltaTime) override;
	virtual AIController* Clone()override { return new AIController(nullptr); }

	virtual void BuildBehaviourTree();
	virtual void SetupBlackBoard();

private:
	BrainTree::BehaviorTree mBehaviourTree;

	Attack* mAttackRef;

	float mAttackRange;
};