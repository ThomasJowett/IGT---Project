#include "GameObject.h"

static unsigned int currentID = 0;

GameObject::GameObject(const char* name, Transform* transform)
	:mName(name), SceneNode(transform)
{
	mIsActive = true;

	mUniqueID = currentID;
	currentID++;
}

GameObject::GameObject(const char * name, Transform * transform, bool active)
	: mName(name), SceneNode(transform)
{
	mIsActive = active;

	mUniqueID = currentID;
	currentID++;
}

//Deep Copy Constructor
//takes the object passed by reference and sets all its members to be equal to this gameobject
GameObject::GameObject(const GameObject & prefab)
	:SceneNode(prefab)
{
	mName = prefab.GetName();

	mFacing = prefab.mFacing;

	for (auto && component : prefab.mComponents)
	{
		AddComponent(component.get()->Clone());
	}
}

GameObject::GameObject()
{
	mName = "Uninitialised";
	mTransform = new Transform();
	mIsActive = false;

	mUniqueID = currentID;
	currentID++;
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

void GameObject::GetAllChildren(std::vector<GameObject*> allGameObjects)
{

}
