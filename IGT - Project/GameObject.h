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
	void Render(Shader* shader);

	void AddComponent(Component* component);

	template <typename ComponentType>
	ComponentType* GetComponent();


	Transform* GetTransform() { return mTransform; }
private:
	std::string mName;
	Transform * mTransform;

	std::vector<Component*>mComponents;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;
};




template<typename ComponentType>
inline ComponentType * GameObject::GetComponent()
{
	for (Component* c : mComponents)
	{
		if (ComponentType* cmp = dynamic_cast<ComponentType*>(c)) {
			return cmp;
		}
	}
	return nullptr;

	//TODO: find better solution for getting a component that dosen't use dynamic_cast
}
#endif // !_GAMEOBJECT_H