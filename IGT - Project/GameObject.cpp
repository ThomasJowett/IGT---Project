#include "GameObject.h"


GameObject::GameObject(std::string name, Transform* transform)
	:mName(name), mTransform(transform)
{
}

GameObject::~GameObject()
{
	for (auto component : mComponents)
	{
		delete component;
	}
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

void GameObject::AddComponent(Component * component)
{
	mComponents.push_back(component);

	if (iRenderable* renderableComp = dynamic_cast<iRenderable*>(component))
		mRenderableComponents.push_back(renderableComp);

	if (iUpdateable* updateableComp = dynamic_cast<iUpdateable*>(component))
		mUpdateableComponents.push_back(updateableComp);
}
