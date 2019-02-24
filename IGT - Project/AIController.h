#pragma once

#include "BrainTree.h"

#include "Attack.h"
//#include "Decorators.h"
#include "Health.h"

class AIController
	:public iUpdateable, public Observer<HealthEvent,GameObject*>
{
public:
	AIController(GameObject* parent);
	~AIController() {};

	void Update(float deltaTime) override;
	virtual AIController* Clone()override { return new AIController(nullptr); }

	virtual void BuildBehaviourTree();
	virtual void SetupBlackBoard();

	void OnNotify(HealthEvent notify, GameObject* gameObject)override;

	void SetParent(GameObject* parent)override;

private:
	BrainTree::BehaviorTree mBehaviourTree;

	Attack* mAttackRef;

	float mAttackRange;
	bool mIsDead = false;
};