#include "GameObject.h"


GameObject::GameObject(std::string name, Transform* transform)
	:mName(name), mTransform(transform)
{
	mIsActive = true;
}

GameObject::GameObject(const GameObject & prefab)
{
	mName = "Uninitialised";
	mTransform = new Transform();
	mIsActive = true;
	
	prefab.Clone(*this);
}

GameObject::GameObject()
{
	mName = "Uninitialised";
	mTransform = new Transform();
	mIsActive = false;
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

void GameObject::AddComponent(Component * component)
{
	mComponents.emplace_back(component);

	if (iRenderable * renderableComp = dynamic_cast<iRenderable*>(component))
		mRenderableComponents.push_back(renderableComp);
	else if (iUpdateable * updateablecomp = dynamic_cast<iUpdateable*>(component))
		mUpdateableComponents.push_back(updateablecomp);

	component->SetParent(this);
}

void GameObject::Clone(GameObject & clonedObject) const
{
	clonedObject.GetTransform()->mPosition = mTransform->mPosition;
	clonedObject.GetTransform()->mRotation = mTransform->mRotation;
	clonedObject.GetTransform()->mScale = mTransform->mScale;

	clonedObject.mName = mName + "copy";

	for (auto && component : mComponents)
	{
		clonedObject.AddComponent(component.get()->Clone());
	}
}
