#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>
#include <memory>

#include "Transform.h"
#include "iComponents.h"

class Component;
class iUpdateable;
class iRenderable;

class GameObject
{
public:
	GameObject(const char* name, Transform* transform);
	GameObject(const GameObject& );
	GameObject();
	virtual ~GameObject();

	void Update(float deltaTime);
	void Render(Shader* shader);

	template<class ComponentType, typename ... Args>
	void AddComponent(Args&& ... params);

	void AddComponent(Component*);

	template<class ComponentType>
	bool RemoveComponent();

	template <typename ComponentType>
	ComponentType* GetComponent() const;

	bool GetActive() {return mIsActive; }
	void SetActive(bool isActive) { mIsActive = isActive; }

	Transform* GetTransform() const { return mTransform; }

	const char* GetName() { return mName; }

	void Clone(GameObject & clonedObject) const;
private:
	const char* mName;
	Transform * mTransform;
	bool mIsActive;

	std::vector < std::unique_ptr<Component>> mComponents;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;
};

template<class ComponentType, typename ...Args>
inline void GameObject::AddComponent(Args && ...params)
{
	mComponents.emplace_back(std::make_unique<ComponentType>(this, std::forward<Args>(params)...));

	if (iRenderable * renderableComp = dynamic_cast<iRenderable*>(mComponents.back().get()))
	{
		mRenderableComponents.push_back(renderableComp);
	}
	else if (iUpdateable * updateableComp = dynamic_cast<iUpdateable*>(mComponents.back().get()))
	{
		mUpdateableComponents.push_back(updateableComp);
	}

	mComponents.back().get()->SetParent(this);
}

template<class ComponentType>
inline bool GameObject::RemoveComponent()
{
	if(mComponents.empty())
		return false;

	for (std::vector< std::unique_ptr<Component>> ::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		if (dynamic_cast<ComponentType*>((*it).get()))
		{
			if (dynamic_cast<iRenderable*>((*it).get()))
			{
				
			}
			mComponents.erase(it);
			return true;
		}
	}

	return false;

	//TODO: find better solution for getting a component that dosen't use dynamic_cast
}

template<typename ComponentType>
inline ComponentType * GameObject::GetComponent() const
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