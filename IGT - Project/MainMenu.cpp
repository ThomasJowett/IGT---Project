#include "MainMenu.h"
#include "Texture2D.h"
#include "GameScreenManager.h"


MainMenu::MainMenu(Transform* transform)
	:UIMenu("MainMenu", transform)
{
	CreateButtons();
}


MainMenu::~MainMenu()
{
}

void MainMenu::CreateButtons()
{
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");

	Button* button = new Button(ButtonTexture, { 0,25 }, { 100, 20 }, "Play");
	button->mButtonID = PLAY;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { 0,0 }, { 100, 20 }, "Options");
	button->mButtonID = OPTIONS;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { 0,-25 }, { 100, 20 }, "Quit");
	button->mButtonID = QUIT;
	button->AddObserver(this);
	mButtons.emplace_back(button);
}

void MainMenu::OnNotify(ButtonEvent event, int ID)
{
	switch (event)
	{
	case ButtonEvent::ON_HOVERED:
		switch (ID)
		{
		case 0:
			break;
		}
		break;
	case ButtonEvent::ON_CLICKED:
		switch (ID)
		{
		case PLAY:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL_1);
			return;
		case OPTIONS:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_OPTIONS);
			return;
		case QUIT:
			SDL_Event sdlEvent;
			sdlEvent.type = SDL_QUIT;
			SDL_PushEvent(&sdlEvent);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
