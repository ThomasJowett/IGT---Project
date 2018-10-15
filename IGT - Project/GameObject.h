#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>
#include <memory>
#include <algorithm>

#include "Transform.h"
#include "iComponents.h"

class Component;
class iUpdateable;
class iRenderable;

class GameObject
{
public:
	GameObject(const char* name, Transform* transform);
	GameObject(const char* name, Transform* transform, bool active);
	GameObject(const GameObject& );
	GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void Render(Shader* shader);

	template<class ComponentType, typename ... Args>
	void AddComponent(Args&& ... params);

	void AddComponent(Component*);

	template<class ComponentType>
	bool RemoveComponent();

	template <typename ComponentType>
	ComponentType* GetComponent() const;

	bool GetActive() const {return mIsActive; }
	void SetActive(bool isActive) { mIsActive = isActive; }

	Transform* GetTransform() const { return mTransform; }

	const char* GetName() { return mName; }

	void SetFacing(FACING facing);

	void Clone(GameObject & clonedObject) const;
private:
	const char* mName;
	Transform * mTransform;
	bool mIsActive;
	FACING mFacing;

	std::vector < std::unique_ptr<Component>> mComponents;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;
};

//Add component by passing type and the parameters
//Will construct the component
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
			bool found = false;
				auto pos = std::find(mRenderableComponents.begin(), mRenderableComponents.end(), (*it).get());
				if (pos != mRenderableComponents.end())
				{
					auto index = std::distance(mRenderableComponents.begin(), pos);
					mRenderableComponents.erase(mRenderableComponents.begin() + index);
					found = true;
				}

			if (!found)
			{
				auto pos = std::find(mUpdateableComponents.begin(), mUpdateableComponents.end(), (*it).get());
				if (pos != mUpdateableComponents.end())
				{
					auto index = std::distance(mUpdateableComponents.begin(), pos);
					mUpdateableComponents.erase(mUpdateableComponents.begin() + index);
				}
			}
			mComponents.erase(it);
			return true;
		}
	}

	return false;

	//TODO: find better solution for getting a component that dosen't use dynamic_cast
}

//Return the first component of that type
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