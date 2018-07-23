#include "MainMenuPawn.h"

#include <iostream>

MainMenuPawn::MainMenuPawn(UIMenu* menu, GameObject* cursor)
	:mMenu(menu), mCursor(cursor)
{
}


MainMenuPawn::~MainMenuPawn()
{
	if (mCursor) delete mCursor;
}

void MainMenuPawn::Up()
{
	mCursor->SetActive(false);

	mMenu->GetButtons()[mCurrentButton]->OnUnHovered();
	mCurrentButton--;

	if (mCurrentButton < 0)
		mCurrentButton = mMenu->GetButtons().size() -1;

	mMenu->GetButtons()[mCurrentButton]->OnHovered();
}

void MainMenuPawn::Down()
{
	mCursor->SetActive(false);

	mMenu->GetButtons()[mCurrentButton]->OnUnHovered();
	mCurrentButton++;

	if (mCurrentButton > mMenu->GetButtons().size() -1)
		mCurrentButton = 0;
	mMenu->GetButtons()[mCurrentButton]->OnHovered();
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
	mMenu->GetButtons()[mCurrentButton]->OnClicked();
}

void MainMenuPawn::AButtonUp()
{
	mMenu->GetButtons()[mCurrentButton]->Execute();
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
	for (int i = 0; i < mMenu->GetButtons().size(); i++)
	{
		if (mMenu->GetButtons()[i]->GetCollisionBox()->ContainsPoint(mMousePosition))
		{
			if (mLeftMouseDown)
			{
				mMenu->GetButtons()[i]->OnClicked();
			}
			else
			{
				mMenu->GetButtons()[i]->OnHovered();
			}
			mCurrentButton = i;
		}
		else
		{
			mMenu->GetButtons()[i]->OnUnHovered();
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
	if (mMenu->GetButtons()[mCurrentButton]->GetCollisionBox()->ContainsPoint(mMousePosition))
		mMenu->GetButtons()[mCurrentButton]->Execute();
}

void MainMenuPawn::SetMenu(UIMenu * menu)
{
	if (menu != mMenu)
	{
		mPreviousMenu = mMenu;

	}
}

