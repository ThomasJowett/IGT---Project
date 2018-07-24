#include "MainMenuPawn.h"

#include <iostream>

MainMenuPawn::MainMenuPawn(UIMenu* menu, GameObject* cursor)
	:mCursor(cursor)
{
}


MainMenuPawn::~MainMenuPawn()
{
	if (mCursor) delete mCursor;
}

void MainMenuPawn::Up()
{
	mCursor->SetActive(false);

	MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->OnUnHovered();
	mCurrentButton--;

	if (mCurrentButton < 0)
		mCurrentButton = MenuManager::GetInstance()->GetCurrentMenu()->GetButtons().size() -1;

	MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->OnHovered();
}

void MainMenuPawn::Down()
{
	mCursor->SetActive(false);

	MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->OnUnHovered();
	mCurrentButton++;

	if (mCurrentButton > MenuManager::GetInstance()->GetCurrentMenu()->GetButtons().size() -1)
		mCurrentButton = 0;
	MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->OnHovered();
}

void MainMenuPawn::Start()
{
	//std::cout << "start\n";
}

void MainMenuPawn::Select()
{
	//std::cout << "select\n";
}

void MainMenuPawn::AButtonDown()
{
	mCursor->SetActive(false);
	MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->OnClicked();
}

void MainMenuPawn::AButtonUp()
{
	MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->Execute();
}

void MainMenuPawn::BButtonDown()
{
	mCursor->SetActive(false);
}

void MainMenuPawn::BButtonUp()
{
}

void MainMenuPawn::MousePosition(float x, float y)
{
	mMousePosition = Vector2D(x, y);
	for (int i = 0; i < MenuManager::GetInstance()->GetCurrentMenu()->GetButtons().size(); i++)
	{
		if (MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[i]->GetCollisionBox()->ContainsPoint(mMousePosition))
		{
			if (mLeftMouseDown)
			{
				MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[i]->OnClicked();
			}
			else
			{
				MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[i]->OnHovered();
			}
			mCurrentButton = i;
		}
		else
		{
			MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[i]->OnUnHovered();
		}
	}

	mCursor->SetActive(true);
	mCursor->GetTransform()->mPosition = Vector3D(x + 8, y - 8, 100);
}

void MainMenuPawn::MouseLeftClick()
{
	mLeftMouseDown = true;
	MousePosition(mMousePosition.x, mMousePosition.y);
}

void MainMenuPawn::MouseLeftUnClick()
{
	mLeftMouseDown = false;
	if (MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->GetCollisionBox()->ContainsPoint(mMousePosition))
		MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[mCurrentButton]->Execute();
}

