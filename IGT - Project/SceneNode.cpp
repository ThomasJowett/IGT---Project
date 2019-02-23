#include "SceneNode.h"

SceneNode::SceneNode()
{
	mParent = nullptr;
	mLeftChild = nullptr;
	mLeftSibling = nullptr;
	mRightSibling = nullptr;

	mTransform = new Transform();

	mIsActive = true;
}

SceneNode::SceneNode(Transform * transform)
	:mTransform(transform)
{
	mParent = nullptr;
	mLeftChild = nullptr;
	mLeftSibling = nullptr;
	mRightSibling = nullptr;

	mIsActive = true;
}

//Deep Copy Constructor
SceneNode::SceneNode(const SceneNode & prefab)
{
	mIsActive = prefab.mIsActive;
	mTransform = new Transform(prefab.GetLocalTransform()->mPosition, prefab.GetLocalTransform()->mRotation, prefab.GetLocalTransform()->mScale);

	if (prefab.mLeftChild)
	{
		mLeftChild = new SceneNode(*prefab.mLeftChild);
		mLeftChild->mParent = this;
	}

	if (prefab.mRightSibling)
		mRightSibling = new SceneNode(*prefab.mRightSibling);

	if (prefab.mLeftSibling)
		mLeftSibling = new SceneNode(*prefab.mLeftSibling);
}

SceneNode::~SceneNode()
{
	if (mLeftChild)
		if (mLeftChild->mRightSibling)
			delete mRightSibling;

	if(mLeftChild) delete mLeftChild;

	RemoveSelf();

	if (mTransform) delete mTransform;
}

void SceneNode::AddChild(SceneNode * child)
{
	//if the node to be attached has a parent deal with the old reference
	if (child->mParent != nullptr)
	{
		if (child->mParent->mLeftChild == child)
		{
			child->mParent->mLeftChild = child->mRightSibling;
		}

		if (child->mLeftSibling != nullptr)
		{
			child->mLeftSibling->mRightSibling = child->mRightSibling;

			if (child->mRightSibling != nullptr)
			{
				child->mRightSibling->mLeftSibling = child->mLeftSibling;
			}
		}
	}

	//if this node has children then move them down the list
	if (mLeftChild != nullptr)
	{
		mLeftChild->mLeftSibling = child;
	}

	//setup the new nodes references
	child->mLeftSibling = nullptr;
	child->mParent = this;
	child->mRightSibling = mLeftChild;
	mLeftChild = child;
}


void SceneNode::RemoveSelf()
{
	//Deal with parents and siblings
	if (mParent != nullptr)
	{
		if (mParent->mLeftChild == this)
		{
			mParent->mLeftChild = mRightSibling;
		}

		if (mLeftSibling != nullptr)
		{
			mLeftSibling->mRightSibling = mRightSibling;

			if (mRightSibling != nullptr)
			{
				mRightSibling->mLeftSibling = mLeftSibling;
			}
		}
	}

	mParent = nullptr;
	mRightSibling = nullptr;
	mLeftSibling = nullptr;
}

void SceneNode::Traverse(Shader * shader, Matrix4x4 & worldMatrix)
{
	//Cache previous world matrix
	Matrix4x4 previousWorldMatrix = worldMatrix;

	//update it's own matrix
	mTransform->UpdateWorldMatrix();

	//add it to the previous
	worldMatrix = worldMatrix * mTransform->GetWorldMatrix();

	mWorldMatrix = worldMatrix;

	//update the shader world matrix
	shader->UpdateMatrixUniform(MODEL_U, worldMatrix, true);

	//render the object
	if (mIsActive)
	{
		Render(shader);

		//if the object has children then traverse them
		if (mLeftChild != nullptr)
			mLeftChild->Traverse(shader, worldMatrix);
	}
	//reset the matrix back to what its parents was
	worldMatrix = previousWorldMatrix;

	//if the object has siblings traverse them
	if (mRightSibling != nullptr)
		mRightSibling->Traverse(shader, worldMatrix);
}

void SceneNode::Traverse(float deltaTime)
{
	if (mIsActive)
	{
		Update(deltaTime);

		if (mLeftChild)
			mLeftChild->Traverse(deltaTime);
	}

	if (mRightSibling)
		mRightSibling->Traverse(deltaTime);
}

Transform * SceneNode::GetWorldTransform() const
{
	SceneNode* parent = mParent;
	Transform transform = *mTransform;

	while (parent)
	{
		transform.mPosition += parent->mTransform->mPosition;
		transform.mRotation += parent->mTransform->mRotation;

		parent = parent->mParent;
	}
	return &transform;
}
