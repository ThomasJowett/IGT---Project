#include "AIController.h"
#include "Tasks.h"
#include "Decorators.h"

AIController::AIController(GameObject* parent)
	:iUpdateable(parent)
{
	if (GetParent())
	{
		BuildBehaviourTree();
		SetupBlackBoard();
		mAttackRef = GetParent()->GetComponent<Attack>();
		GetParent()->GetComponent<Health>()->AddObserver(this);

		GetParent()->GetComponent<Collider>()->AddObserver(this);
	}
}

void AIController::Update(float deltaTime)
{
	if (!mIsDead)
	{
		if (mTarget)
		{
			Vector2D parentLocation = GetParent()->GetWorldTransform().mPosition;
			Vector2D targetLocation = mTarget->GetWorldTransform().mPosition;

			float distanceToTarget = Vector2D::Distance(targetLocation, parentLocation);
			if (distanceToTarget < mAttackRange)
			{
				mBehaviourTree.getBlackboard()->setBool("IsInAttackRange", true);
			}
			else if (distanceToTarget > mAgroRange)
			{
				mTarget = nullptr;
				mBehaviourTree.getBlackboard()->setBool("IsInAttackRange", false);
				mBehaviourTree.getBlackboard()->setVector2D("MoveToLocation", mStartingLocation);
			}
			else
			{
				mBehaviourTree.getBlackboard()->setBool("IsInAttackRange", false);
				mBehaviourTree.getBlackboard()->setVector2D("MoveToLocation", targetLocation);
			}
		}

		mBehaviourTree.update(deltaTime);
	}
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

	auto decorator = std::make_shared<BlackboardBool>(mBehaviourTree.getBlackboard(), "IsInAttackRange", true);

	selector->addChild(decorator);
	decorator->setChild(sequence);
	selector->addChild(std::make_shared<MoveTo>(mBehaviourTree.getBlackboard(), 10.0f, "MoveToLocation", GetParent()));
	sequence->addChild(std::make_shared<AttackTask>(GetParent()));
	sequence->addChild(std::make_shared<Wait>(1.5f));
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
	//Set the initial move to location to the position it is currently at
	mBehaviourTree.getBlackboard()->setVector2D("MoveToLocation", Vector2D());
	//mBehaviourTree.getBlackboard()->setVector2D("MoveToLocation", { 1380, -127 });
	mBehaviourTree.getBlackboard()->setBool("IsInAttackRange", false);
}

void AIController::OnNotify(HealthEvent notify, GameObject * gameObject)
{
	switch (notify)
	{
	case HealthEvent::ON_DEATH:
		mIsDead = true;

		break;

	default:
		break;
	}
}

void AIController::OnNotify(OverlapEvent notify, GameObject * overlappedWith)
{
	switch (notify)
	{
	case OverlapEvent::BEGIN_OVERLAP:
		if (!strcmp(overlappedWith->GetName(), "BarbarianCharacter") || !strcmp(overlappedWith->GetName(), "ArcherCharacter"))
		{
			mTarget = overlappedWith;
			std::cout << mTarget->GetName() << std::endl;

			mStartingLocation = GetParent()->GetWorldTransform().mPosition;
		}
	}
}

void AIController::SetParent(GameObject * parent)
{
	Component::SetParent(parent);
	if (GetParent())
	{
		BuildBehaviourTree();
		SetupBlackBoard();
		
		mAttackRef = GetParent()->GetComponent<Attack>();
		GetParent()->GetComponent<Health>()->AddObserver(this);
		GetParent()->GetComponent<Collider>()->AddObserver(this);
	}
}
