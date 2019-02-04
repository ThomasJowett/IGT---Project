#include "MainMenuPawn.h"

#include <iostream>
#include <SDL.h>
#include "Cursor.h"

MainMenuPawn::MainMenuPawn()
{
}


MainMenuPawn::~MainMenuPawn()
{
}

void MainMenuPawn::Up()
{
	SDL_ShowCursor(SDL_DISABLE);

	MenuManager::GetInstance()->GetCurrentMenu()->Up();
}

void MainMenuPawn::Down()
{
	SDL_ShowCursor(SDL_DISABLE);

	MenuManager::GetInstance()->GetCurrentMenu()->Down();
}

void MainMenuPawn::Left()
{
	SDL_ShowCursor(SDL_DISABLE);

	MenuManager::GetInstance()->GetCurrentMenu()->Left();
}

void MainMenuPawn::Right()
{
	SDL_ShowCursor(SDL_DISABLE);

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
	SDL_ShowCursor(SDL_DISABLE);
	MenuManager::GetInstance()->GetCurrentWidget()->OnPressed();
}

void MainMenuPawn::AButtonUp()
{
	MenuManager::GetInstance()->GetCurrentWidget()->OnReleased();
}

void MainMenuPawn::BButtonDown()
{
	SDL_ShowCursor(SDL_DISABLE);
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
					widget->OnDragged(mMousePosition);
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

	SDL_ShowCursor(SDL_ENABLE);
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
	iInput::OnPossessed(controllerID);
	SDL_ShowCursor(SDL_ENABLE);
	Cursor::CustomCursorType("Cursor_Arrow");
}

void MainMenuPawn::OnUnPossesed()
{
	iInput::OnUnPossesed();
	//SDL_ShowCursor(SDL_DISABLE);
	Cursor::CustomCursorType("Cursor_Crosshairs");
}

