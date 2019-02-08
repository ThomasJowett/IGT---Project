#include "PauseMenu.h"
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "Cursor.h"

PauseMenu::PauseMenu(Transform* transform, iInput* playerPawn)
	:UIMenu("Pause Menu", transform), mPlayerPawn(playerPawn)
{
	CreateWidgets();

	SetActive(false);
}

void PauseMenu::CreateWidgets()
{
	GLuint ButtonTexture = Texture2D::GetTexture2D("Images/ButtonsMenu.png");

	Button* button = new Button(ButtonTexture, { 0.5, 0.5 }, { 0,25 }, { 100, 20 }, "Resume");
	button->mWidgetData.ID = 0;
	button->AddObserver(this);
	mWidgets.emplace_back(button);
	AddChild(button);

	button = new Button(ButtonTexture, { 0.5, 0.5 }, { 0, -25 }, { 100, 20 }, "Quit");
	button->mWidgetData.ID = 1;
	button->AddObserver(this);
	mWidgets.emplace_back(button);
	AddChild(button);
}

void PauseMenu::OnNotify(WidgetEvent event, WidgetEventData data)
{
	switch (event)
	{
	case WidgetEvent::ON_HOVERED:
		switch (data.ID)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
		break;
	case WidgetEvent::ON_CLICKED:
		switch (data.ID)
		{
		case 0:
			MenuManager::GetInstance()->ShowCurrentMenu(false);
			GameScreenManager::GetInstance()->GetCurrentScreen()->GetPlayerControllers()[0]->PossesPawn(mPlayerPawn);
			Cursor::CustomCursorType("Cursor_Crosshairs");
			GameScreenManager::GetInstance()->ResumeGame();
			return;
		case 1:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_MENU);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void PauseMenu::Back()
{
	MenuManager::GetInstance()->ShowCurrentMenu(false);
	GameScreenManager::GetInstance()->GetCurrentScreen()->GetPlayerControllers()[0]->PossesPawn(mPlayerPawn);
	Cursor::CustomCursorType("Cursor_Crosshairs");
	SDL_ShowCursor(SDL_ENABLE);
	GameScreenManager::GetInstance()->ResumeGame();
}
