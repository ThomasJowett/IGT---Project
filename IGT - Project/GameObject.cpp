#include "GameObject.h"


GameObject::GameObject(std::string name, Transform* transform)
	:mName(name), mTransform(transform)
{
	mIsActive = true;
}

GameObject::~GameObject()
{
	mComponents.clear();
}

void GameObject::Update(float deltaTime)
{
	for (auto component : mUpdateableComponents)
	{
		component->Update(deltaTime);
	}
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