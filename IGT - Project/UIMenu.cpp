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
	for (UIWidget* widget : mWidgets)
		delete widget;
}

void UIMenu::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
	
	if (GetActive())
	{
		for (UIWidget* widget : mWidgets)
		{
			if (widget->GetActive())
				widget->Update(deltaTime);
		}
	}
}

void UIMenu::SetCurrentWidget(unsigned int widgetID)
{
	if (widgetID >= 0 && widgetID < mWidgets.size())
	{
		if (mWidgets[widgetID]->GetIsFocusable())
			mCurrentWidget = widgetID;
	}
}

void UIMenu::Up()
{
	mWidgets[mCurrentWidget]->OnUnHovered();	
	mCurrentWidget--;

	if (mCurrentWidget < 0)
		mCurrentWidget = mWidgets.size() - 1;

	if (!mWidgets[mCurrentWidget]->GetIsFocusable() || !mWidgets[mCurrentWidget]->GetActive())
	{
		Up();
	}
	
	mWidgets[mCurrentWidget]->OnHovered();
}

void UIMenu::Down()
{
	mWidgets[mCurrentWidget]->OnUnHovered();
	mCurrentWidget++;

	if (mCurrentWidget > mWidgets.size() - 1)
		mCurrentWidget = 0;

	if (!mWidgets[mCurrentWidget]->GetIsFocusable() || !mWidgets[mCurrentWidget]->GetActive())
	{
		Down();
	}

	mWidgets[mCurrentWidget]->OnHovered();
}

void UIMenu::Back()
{
	MenuManager::GetInstance()->ChangeToPreviousMenu();
}
