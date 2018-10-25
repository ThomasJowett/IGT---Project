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
	virtual void Render(Shader * shader) = 0;
	void Traverse(Shader* shader, Matrix4x4 & worldMatrix);

	Transform* GetLocalTransform() const { return mTransform; };
	Transform* GetWorldTransform() const;

protected:
	Transform* mTransform;

	SceneNode * mParent;
	SceneNode * mLeftChild;
	SceneNode * mRightSibling;
	SceneNode * mLeftSibling;
};