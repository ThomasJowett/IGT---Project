#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <string>
#include <vector>

#include "Transform.h"
#include "iComponents.h"

class GameObject
{
public:
	GameObject(std::string name, Transform* transform);
	~GameObject();

	void Update(float deltaTime);
	void Render();

	void AddUpdateableComponent(iUpdateable* component);
	void AddRenderableComponent(iRenderable* component);
private:
	std::string mName;
	Transform * mTransform;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;

	//Collider mCollider;
	//PhysicsComponent mPhysicsComp;
};

#endif // !_GAMEOBJECT_H