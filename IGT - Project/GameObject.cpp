#include "GameObject.h"


GameObject::GameObject(const char* name, Transform* transform)
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
	if (mTransform) delete mTransform;

	mRenderableComponents.clear();

	mUpdateableComponents.clear();

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
		mRenderableComponents.emplace_back(renderableComp);
	else if (iUpdateable * updateablecomp = dynamic_cast<iUpdateable*>(component))
		mUpdateableComponents.emplace_back(updateablecomp);

	component->SetParent(this);
}

void GameObject::SetFacing(FACING facing)
{
	if (mFacing != facing)
	{
		GetTransform()->mScale.x *= -1.0f;
		mFacing = facing;
	}
}

void GameObject::Clone(GameObject & clonedObject) const
{
	clonedObject.GetTransform()->mPosition = mTransform->mPosition;
	clonedObject.GetTransform()->mRotation = mTransform->mRotation;
	clonedObject.GetTransform()->mScale = mTransform->mScale;

	clonedObject.mName = mName;

	for (auto && component : mComponents)
	{
		clonedObject.AddComponent(component.get()->Clone());
	}
}
