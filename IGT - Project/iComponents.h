#ifndef _ICOMPONENTS_H

#define _ICOMPONENTS_H

#include "Shader.h"
#include "GameObject.h"

class GameObject;

class Component
{
public:
	Component(GameObject* parent)
		:mParent(parent){}
	virtual ~Component() = default;

	virtual Component* Clone() = 0;

	void SetParent(GameObject* parent) { mParent = parent; }
	GameObject* GetParent() const { return mParent; }

	bool GetActive() const { return mIsActive; }
	void SetActive(bool isActive) { mIsActive = isActive; }
private:
	GameObject * mParent;
	bool mIsActive = true;
};

class iUpdateable : public Component
{
public:
	iUpdateable(GameObject* parent)
		:Component(parent) {}
	virtual void Update(float deltaTime) = 0;
};

class iRenderable : public Component
{
public:
	iRenderable(GameObject* parent)
		:Component(parent) {}
	virtual void Render(Shader* shader) = 0;
};

#endif // !_ICOMPONENTS_H