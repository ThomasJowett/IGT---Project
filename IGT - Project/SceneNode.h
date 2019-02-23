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