#include "SceneNode.h"

SceneNode::SceneNode()
{
	mParent = nullptr;
	mLeftChild = nullptr;
	mLeftSibling = nullptr;
	mRightSibling = nullptr;

	mTransform = new Transform();
}

SceneNode::SceneNode(Transform * transform)
{
	mParent = nullptr;
	mLeftChild = nullptr;
	mLeftSibling = nullptr;
	mRightSibling = nullptr;
}

SceneNode::~SceneNode()
{
	RemoveSelf();
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
}

void SceneNode::Traverse(Shader * shader, Matrix4x4 & worldMatrix)
{
	//Cache previous world matrix
	Matrix4x4 previousWorldMatrix = worldMatrix;

	//update it's own matrix
	mTransform->UpdateWorldMatrix();

	//add it to the previous
	worldMatrix = worldMatrix * mTransform->GetWorldMatrix();

	//update the shader world matrix
	shader->UpdateMatrixUniform(MODEL_U, worldMatrix, true);

	//render the object
	Render(shader);

	//if the object has children then traverse them
	if (mLeftChild != nullptr)
		mLeftChild->Traverse(shader, worldMatrix);

	//if not then reset the matrix back to what its parents was
	else
		worldMatrix = previousWorldMatrix;

	//if the object has siblings traverse them
	if (mRightSibling != nullptr)
		mRightSibling->Traverse(shader, worldMatrix);
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
