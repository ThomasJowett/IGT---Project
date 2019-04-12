#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>
#include <memory>
#include <algorithm>

#include "SceneNode.h"
#include "iComponents.h"

class Component;
class iUpdateable;
class iRenderable;

enum Layer
{
	SORTED,
	FOREGROUND,
	BACKGROUND
};

class GameObject :public SceneNode
{
public:
	GameObject(const char* name, Transform* transform);
	GameObject(const char* name, Transform* transform, bool active);
	GameObject(const GameObject& );
	GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime) override;
	virtual void Render(Shader* shader) override;

	template<class ComponentType, typename ... Args>
	void AddComponent(Args&& ... params);

	void AddComponent(Component*);

	template<class ComponentType>
	bool RemoveComponent();

	template <typename ComponentType>
	ComponentType* GetComponent() const;

	template <typename ComponentType>
	ComponentType* GetComponent(int number) const;

	template <typename ComponentType>
	std::vector<ComponentType*> GetAllComponents() const;

	const char* GetName() const { return mName; }

	void SetFacing(FACING facing);

	Layer GetLayer() const { return mLayer; }
	void SetLayer(Layer layer) { mLayer = layer; }

	unsigned int GetUniqueID() { return mUniqueID; }

	bool Destroy();

	bool GetDeathFlag() { return mDeathFlag; }
private:
	const char* mName;

	FACING mFacing;

	Layer mLayer;

	std::vector < std::unique_ptr<Component>> mComponents;

	std::vector<iUpdateable*>mUpdateableComponents;
	std::vector<iRenderable*>mRenderableComponents;

	unsigned int mUniqueID;

	bool mDeathFlag = false;
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

template<typename ComponentType>
inline ComponentType * GameObject::GetComponent(int number) const
{
	int counter = 0;
	for (auto && component : mComponents)
	{
		if (ComponentType * returnComponent = dynamic_cast<ComponentType*>(component.get()))
		{
			counter++;
			if (counter == number)
			{
				return returnComponent;
			}
		}
	}
	return nullptr;
}

template<typename ComponentType>
inline std::vector<ComponentType*> GameObject::GetAllComponents() const
{
	std::vector<ComponentType*> returnComponents;

	for (auto && component : mComponents)
	{
		if (ComponentType * returnComponent = dynamic_cast<ComponentType*>(component.get()))
			returnComponents.push_back(returnComponent);
	}
	return returnComponents;
}

#endif // !_GAMEOBJECT_H