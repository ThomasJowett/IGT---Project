#include "UIMenu.h"

UIMenu::UIMenu(const char* name, Transform* transform)
	:GameObject(name, transform)
{
}

UIMenu::UIMenu(const char * name, Transform * transform, bool active)
	:GameObject(name, transform, active)
{
}

UIMenu::~UIMenu()
{
	//delete all buttons
}

void UIMenu::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	for (Button* button : mButtons)
	{
		button->Update(deltaTime);
	}
}

void UIMenu::Render(Shader * shader)
{
	GameObject::Render(shader);

	for (Button* button : mButtons)
	{
		button->Render(shader);
	}
}
