#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <string>
#include <vector>
#include <memory>

#include "Transform.h"
#include "iComponents.h"

class GameObject
{
public:
	std::vector < std::unique_ptr<Component>> mComponents;
public:
	GameObject(std::string name, Transform* transform);
	~GameObject();

	void Update(float deltaTime);
	void Render(Shader* shader);

	//void AddComponent(Component* component);

	template<class ComponentType, typename ... Args>
	void AddComponent(Args&& ... params);

	template <typename ComponentType>
	ComponentType& GetComponent();

	bool GetActive() {return mIsActive; }
	void SetActive(bool isActive) { mIsActive = isActive; }

	Transform* GetTransform() { return mTransform; }
private:
	std::string mName;
	Transform * mTransform;
	bool mIsActive;

	
	//std::vector<Component*>mComponents;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;
};

template<class ComponentType, typename ...Args>
inline void GameObject::AddComponent(Args && ...params)
{
	mComponents.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));
}

template<typename ComponentType>
inline ComponentType & GameObject::GetComponent()
{
	for (auto && component : mComponents)
	{
		if (component->IsClassType(Component::Type))
			return *static_cast<ComponentType *>(component.get());
	}
	return *std::unique_ptr< ComponentType >(nullptr);

	//TODO: find better solution for getting a component that dosen't use dynamic_cast
}
#endif // !_GAMEOBJECT_H