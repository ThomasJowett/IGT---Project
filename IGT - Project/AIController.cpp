#include "AIController.h"

AIController::AIController(GameObject* parent)
	:iUpdateable(parent)
{
	BuildBehaviourTree();
	SetupBlackBoard();

	mAttackRef = GetParent()->GetComponent<Attack>();
}

void AIController::Update(float deltaTime)
{
	mBehaviourTree.update(deltaTime);
}

void AIController::BuildBehaviourTree()
{
	//mBehaviourTree = BrainTree::Builder()
	//	.composite<BrainTree::Sequence>()
	//		.leaf<MoveTo>(mBehaviourTree.getBlackboard(), 10.0f, "MoveToLocation")
	//		.leaf<Wait>(2.0f)
	//	.end()
	//	.build();
	auto selector = std::make_shared<BrainTree::Selector>();
	
	auto sequence = std::make_shared<BrainTree::Sequence>();

	auto decorator = std::make_shared<isinrangedecorator>//TODO make the decorator
	selector->addChild(sequence);
	selector->addChild(std::make_shared<MoveTo>(mBehaviourTree.getBlackboard(), 10.0f, "MoveToLocation"));
	sequence->addChild(std::make_shared<AttackTask>(GetParent()));
	sequence->addChild(std::make_shared<Wait>(0.4f));
	mBehaviourTree.setRoot(selector);

	/*
					Root
					 |
					 |
				  Selector
			    /          \
			   /            \
			  /              \
Is in attack range      Is not in attack range
==================      ======================
	Sequence			      MoveTo(player)
	/           \
   /             \
  /               \
 Attack			Wait(0.4)
*/
}

void AIController::SetupBlackBoard()
{
	mBehaviourTree.getBlackboard()->setVector2D("MoveToLocation", Vector2D());
	mBehaviourTree.getBlackboard()->setBool("IsInAttackRange", false);
}