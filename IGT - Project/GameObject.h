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
	GameObject(std::string name, Transform* transform);
	~GameObject();

	void Update(float deltaTime);
	void Render(Shader* shader);

	template<class ComponentType, typename ... Args>
	void AddComponent(Args&& ... params);

	template <typename ComponentType>
	ComponentType* GetComponent();

	bool GetActive() {return mIsActive; }
	void SetActive(bool isActive) { mIsActive = isActive; }

	Transform* GetTransform() { return mTransform; }
private:
	std::string mName;
	Transform * mTransform;
	bool mIsActive;

	std::vector < std::unique_ptr<Component>> mComponents;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;
};

template<class ComponentType, typename ...Args>
inline void GameObject::AddComponent(Args && ...params)
{
	mComponents.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));

	if (iRenderable * renderableComp = dynamic_cast<iRenderable*>(mComponents.back().get()))
		mRenderableComponents.push_back(renderableComp);
	else if (iUpdateable * updateableComp = dynamic_cast<iUpdateable*>(mComponents.back().get()))
		mUpdateableComponents.push_back(updateableComp);
}

template<typename ComponentType>
inline ComponentType * GameObject::GetComponent()
{
	for (auto && component : mComponents)
	{
		if (ComponentType * returnComponent = dynamic_cast<ComponentType*>(component.get()))
			return returnComponent;
	}
	return nullptr;

	//TODO: find better solution for getting a component that dosen't use dynamic_cast
}
#endif // !_GAMEOBJECT_H