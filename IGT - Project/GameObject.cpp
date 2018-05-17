#include "GameObject.h"


GameObject::GameObject(std::string name, Transform* transform)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update(float deltaTime)
{
	for (auto component : mUpdateableComponents)
	{
		component->Update(deltaTime);
	}

	//if(mPhysicsComp)
		//mPhysicsComp->Update(deltaTime);
}

void GameObject::Render()
{
	for (auto component : mRenderableComponents)
	{
		component->Render();
	}
}

void GameObject::AddUpdateableComponent(iUpdateable* component)
{
	mUpdateableComponents.push_back(component);
}

void GameObject::AddRenderableComponent(iRenderable* component)
{
	mRenderableComponents.push_back(component);
}

