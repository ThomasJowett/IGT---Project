#include "MainMenuPawn.h"

#include <iostream>

MainMenuPawn::MainMenuPawn(std::vector<Button*> buttons, GameObject* cursor)
	:mButtons(buttons), mCursor(cursor)
{
}


MainMenuPawn::~MainMenuPawn()
{
}

void MainMenuPawn::Up()
{
	mCursor->SetActive(false);

	mButtons[mCurrentButton]->OnUnHovered();
	mCurrentButton--;

	if (mCurrentButton < 0)
		mCurrentButton = mButtons.size() -1;

	mButtons[mCurrentButton]->OnHovered();
}

void MainMenuPawn::Down()
{
	mCursor->SetActive(false);

	mButtons[mCurrentButton]->OnUnHovered();
	mCurrentButton++;

	if (mCurrentButton > mButtons.size() -1)
		mCurrentButton = 0;
	mButtons[mCurrentButton]->OnHovered();
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
	mButtons[mCurrentButton]->OnClicked();
}

void MainMenuPawn::AButtonUp()
{
	mButtons[mCurrentButton]->Execute();
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
	for (int i = 0; i < mButtons.size(); i++)
	{
		if (mButtons[i]->GetCollisionBox()->ContainsPoint(mMousePosition))
		{
			if (mLeftMouseDown)
			{
				mButtons[i]->OnClicked();
			}
			else
			{
				mButtons[i]->OnHovered();
			}
			mCurrentButton = i;
		}
		else
		{
			mButtons[i]->OnUnHovered();
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
	if (mButtons[mCurrentButton]->GetCollisionBox()->ContainsPoint(mMousePosition))
		mButtons[mCurrentButton]->Execute();
}

