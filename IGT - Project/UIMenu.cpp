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
	for (Button* button : mButtons)
		delete button;
}

void UIMenu::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	
	for (Button* button : mButtons)
	{
		if (button->GetActive())
			button->Update(deltaTime);
	}
}

void UIMenu::Render(Shader * shader)
{
	GameObject::Render(shader);

	for (Button* button : mButtons)
	{
		if(button->GetActive())
			button->Render(shader);
	}
}

void UIMenu::SetCurrentButton(int button)
{
	if (button >= 0 && button < mButtons.size())
		mCurrentButton = button;
}

void UIMenu::Up()
{
	mButtons[mCurrentButton]->OnUnHovered();
	mCurrentButton--;

	if (mCurrentButton < 0)
		mCurrentButton = mButtons.size() - 1;

	mButtons[mCurrentButton]->OnHovered();
}

void UIMenu::Down()
{
	mButtons[mCurrentButton]->OnUnHovered();
	mCurrentButton++;

	if (mCurrentButton > mButtons.size() - 1)
		mCurrentButton = 0;

	mButtons[mCurrentButton]->OnHovered();
}

void UIMenu::Back()
{
	MenuManager::GetInstance()->ChangeToPreviousMenu();
}
