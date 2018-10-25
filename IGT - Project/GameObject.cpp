#include "GameObject.h"


GameObject::GameObject(const char* name, Transform* transform)
	:mName(name), SceneNode(transform)
{
	mIsActive = true;
}

GameObject::GameObject(const char * name, Transform * transform, bool active)
	: mName(name), SceneNode(transform)
{
	mIsActive = active;
}

GameObject::GameObject(const GameObject & prefab)
{
	mName = "Uninitialised";
	mTransform = new Transform();
	
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
	mRenderableComponents.clear();

	mUpdateableComponents.clear();

	mComponents.clear();
}

//Updates all the objects updateable components
void GameObject::Update(float deltaTime)
{
	if (mIsActive)
	{
		for (auto component : mUpdateableComponents)
		{
			if (component->GetActive())
				component->Update(deltaTime);
		}
	}
}

//Displays all the objects renderable components on the screen
void GameObject::Render(Shader* shader)
{
	if (mIsActive)
	{
		if (mName == "Options Menu" && mIsActive)
		{
			float stop = 2 + 3;
		}

		//mTransform->UpdateWorldMatrix();
		//shader->UpdateMatrixUniform(MODEL_U, mTransform->GetWorldMatrix(),true);

		for (auto component : mRenderableComponents)
		{
			if (component->GetActive())
				component->Render(shader);
		}
	}
}

//Adds a component if the component has already been constructed
void GameObject::AddComponent(Component * component)
{
	mComponents.emplace_back(component);

	if (iRenderable * renderableComp = dynamic_cast<iRenderable*>(component))
		mRenderableComponents.emplace_back(renderableComp);
	else if (iUpdateable * updateablecomp = dynamic_cast<iUpdateable*>(component))
		mUpdateableComponents.emplace_back(updateablecomp);

	component->SetParent(this);
}

//Set the x scale of the transform of the game object 
void GameObject::SetFacing(FACING facing)
{
	if (mFacing != facing)
	{
		GetTransform()->mScale.x *= -1.0f;
		mFacing = facing;
	}
}

//takes the object passed by reference and sets all its members to be equal to this gameobject
void GameObject::Clone(GameObject & clonedObject) const
{
	clonedObject.GetTransform()->mPosition = mTransform->mPosition;
	clonedObject.GetTransform()->mRotation = mTransform->mRotation;
	clonedObject.GetTransform()->mScale = mTransform->mScale;

	clonedObject.mName = mName;
	clonedObject.mIsActive = mIsActive;
	clonedObject.mFacing = mFacing;

	for (auto && component : mComponents)
	{
		clonedObject.AddComponent(component.get()->Clone());
	}
}
