#include "MainMenuPawn.h"

#include <iostream>

MainMenuPawn::MainMenuPawn(GameObject* cursor)
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

	MenuManager::GetInstance()->GetCurrentMenu()->Up();
}

void MainMenuPawn::Down()
{
	mCursor->SetActive(false);

	MenuManager::GetInstance()->GetCurrentMenu()->Down();
}

void MainMenuPawn::Left()
{
	mCursor->SetActive(false);

	MenuManager::GetInstance()->GetCurrentMenu()->Left();
}

void MainMenuPawn::Right()
{
	mCursor->SetActive(false);

	MenuManager::GetInstance()->GetCurrentMenu()->Right();
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
	MenuManager::GetInstance()->GetCurrentButton()->OnClicked();
}

void MainMenuPawn::AButtonUp()
{
	MenuManager::GetInstance()->GetCurrentButton()->Execute();
}

void MainMenuPawn::BButtonDown()
{
	mCursor->SetActive(false);
}

void MainMenuPawn::BButtonUp()
{
	MenuManager::GetInstance()->GetCurrentMenu()->Back();
}

void MainMenuPawn::RightBumper()
{
	MenuManager::GetInstance()->GetCurrentMenu()->Next();
}

void MainMenuPawn::LeftBumper()
{
	MenuManager::GetInstance()->GetCurrentMenu()->Previous();
}

void MainMenuPawn::MousePosition(float x, float y)
{
	mMousePosition = Vector2D(x, y);
	for (int i = 0; i < MenuManager::GetInstance()->GetCurrentMenu()->GetButtons().size(); i++)
	{
		Button* button = MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[i];

		if (button->GetActive())
		{
			if (button->GetCollisionBox()->ContainsPoint(mMousePosition))
			{
				if (mLeftMouseDown)
				{
					button->OnClicked();
				}
				else
				{
					button->OnHovered();
				}
				MenuManager::GetInstance()->GetCurrentMenu()->SetCurrentButton(i);
			}
			else
			{
				MenuManager::GetInstance()->GetCurrentMenu()->GetButtons()[i]->OnUnHovered();
			}
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

	if (MenuManager::GetInstance()->GetCurrentButton()->GetCollisionBox()->ContainsPoint(mMousePosition))
		MenuManager::GetInstance()->GetCurrentButton()->Execute();
}

void MainMenuPawn::OnPossessed(int controllerID)
{
	mCursor->SetActive(true);
}

void MainMenuPawn::OnUnPossesed()
{
	mCursor->SetActive(false);
}

