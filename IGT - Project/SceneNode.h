#pragma once

#include "Transform.h"
#include "Shader.h"

class SceneNode
{
public:
	SceneNode();
	SceneNode(Transform * transform);
	SceneNode(const SceneNode&);
	virtual ~SceneNode();

	void AddChild(SceneNode * child);
	void RemoveSelf();

	virtual void Render(Shader * shader) {}
	virtual void Update(float deltaTime) {}

	void Traverse(Shader* shader, Matrix4x4 & worldMatrix);
	void Traverse(float deltaTime);

	Transform* GetLocalTransform() const { return mLocalTransform; };
	Transform GetWorldTransform() const;

	//Vector3D GetWorldLocation() const;

	void SetActive(bool isActive) { mIsActive = isActive; }
	bool GetActive() { return mIsActive; }

	//only updates on render, so will be the position of node in previous frame
	Matrix4x4 GetWorldMatrix() { return mWorldMatrix; }

	SceneNode* GetParent() const { return mParent; }

	void AddAllChildrenToList(std::vector<SceneNode*> &list);

protected:
	Transform* mLocalTransform;

	Matrix4x4 mWorldMatrix;

	bool mIsActive;

	SceneNode * mParent = nullptr;
	SceneNode * mLeftChild = nullptr;
	SceneNode * mRightSibling = nullptr;
	SceneNode * mLeftSibling = nullptr;
};