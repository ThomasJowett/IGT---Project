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
	MenuManager::GetInstance()->GetCurrentWidget()->OnPressed();
}

void MainMenuPawn::AButtonUp()
{
	MenuManager::GetInstance()->GetCurrentWidget()->OnReleased();
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
	for (int i = 0; i < MenuManager::GetInstance()->GetCurrentMenu()->GetWidgets().size(); i++)
	{
		UIWidget* widget = MenuManager::GetInstance()->GetCurrentMenu()->GetWidgets()[i];

		if (widget->GetActive())
		{
			if (widget->ContainsMouse(mMousePosition))
			{
				if (mLeftMouseDown)
				{
					widget->OnPressed();
				}
				else
				{
					widget->OnHovered();
				}
				MenuManager::GetInstance()->GetCurrentMenu()->SetCurrentWidget(i);
			}
			else
			{
				MenuManager::GetInstance()->GetCurrentMenu()->GetWidgets()[i]->OnUnHovered();
			}
		}
	}

	mCursor->SetActive(true);
	mCursor->GetTransform()->mPosition = Vector3D(x + 8, y - 8, 0);
}

void MainMenuPawn::MouseLeftClick()
{
	mLeftMouseDown = true;
	MousePosition(mMousePosition.x, mMousePosition.y);
}

void MainMenuPawn::MouseLeftUnClick()
{
	mLeftMouseDown = false;

	if (MenuManager::GetInstance()->GetCurrentWidget()->ContainsMouse(mMousePosition))
		MenuManager::GetInstance()->GetCurrentWidget()->OnReleased();
}

void MainMenuPawn::OnPossessed(int controllerID)
{
	mCursor->SetActive(true);
}

void MainMenuPawn::OnUnPossesed()
{
	mCursor->SetActive(false);
}

