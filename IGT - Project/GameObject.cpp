#include "GameObject.h"


GameObject::GameObject(std::string name, Transform* transform)
	:mName(name), mTransform(transform)
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

void GameObject::Render(Shader* shader)
{
	mTransform->UpdateWorldMatrix();
	shader->UpdateWorld(mTransform);

	for (auto component : mRenderableComponents)
	{
		component->Render(shader);
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