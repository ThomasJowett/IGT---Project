#include "PauseMenu.h"
#include "Texture2D.h"
#include "GameScreenManager.h"

PauseMenu::PauseMenu(Transform* transform)
	:UIMenu("Pause Menu", transform)
{
	CreateButtons();

	SetActive(false);
}

void PauseMenu::CreateButtons()
{
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");

	Button* button = new Button(ButtonTexture, { 0,25 }, { 100, 20 }, "Resume");
	button->mButtonID = 0;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { 0,-25 }, { 100, 20 }, "Quit");
	button->mButtonID = 1;
	button->AddObserver(this);
	mButtons.emplace_back(button);
}

void PauseMenu::OnNotify(ButtonEvent event, int ID)
{
	switch (event)
	{
	case ButtonEvent::ON_HOVERED:
		switch (ID)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
		break;
	case ButtonEvent::ON_CLICKED:
		switch (ID)
		{
		case 0:
			MenuManager::GetInstance()->ShowCurrentMenu(false);
			return;
		case 1:
			MenuManager::GetInstance()->ChangeToMenu(1);
			return;
		case 2:
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
