#pragma once

#include "Transform.h"
#include "Shader.h"

class SceneNode
{
public:
	SceneNode();
	SceneNode(Transform * transform);
	virtual ~SceneNode();

	void AddChild(SceneNode * child);
	void RemoveSelf();
	virtual void Render(Shader * shader) {}
	void Traverse(Shader* shader, Matrix4x4 & worldMatrix);

	Transform* GetLocalTransform() const { return mTransform; };
	Transform* GetWorldTransform() const;

	void SetActive(bool isActive) { mIsActive = isActive; }
	bool GetActive() { return mIsActive; }

	Matrix4x4 GetWorldMatrix() { return mWorldMatrix; }

protected:
	Transform* mTransform;

	Matrix4x4 mWorldMatrix;

	bool mIsActive;

	SceneNode * mParent;
	SceneNode * mLeftChild;
	SceneNode * mRightSibling;
	SceneNode * mLeftSibling;
};