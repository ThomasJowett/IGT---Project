#include "MainMenu.h"
#include "Texture2D.h"
#include "GameScreenManager.h"

MainMenu::MainMenu(Transform* transform)
	:UIMenu("MainMenu", transform)
{
	CreateWidgets();

	//TODO: add the button prompts (A and B) to screen somewhere
	//AddComponent<Sprite>();
}


MainMenu::~MainMenu()
{
	
}

void MainMenu::CreateWidgets()
{
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");

	Button* button = new Button(ButtonTexture, { 0.5,0.5 }, { 0, 25 }, { 100, 20 }, "Play");
	button->mWidgetData.ID = PLAY;
	button->AddObserver(this);
	mWidgets.emplace_back(button);

	button = new Button(ButtonTexture, { 0.5,0.5 }, { 0,0 }, { 100, 20 }, "Options");
	button->mWidgetData.ID = OPTIONS;
	button->AddObserver(this);
	mWidgets.emplace_back(button);

	button = new Button(ButtonTexture, { 0.5,0.5 }, { 0,-25 }, { 100, 20 }, "Quit");
	button->mWidgetData.ID = QUIT;
	button->AddObserver(this);
	mWidgets.emplace_back(button);
}

void MainMenu::OnNotify(WidgetEvent event, WidgetEventData data)
{
	switch (event)
	{
	case WidgetEvent::ON_HOVERED:
		switch (data.ID)
		{
		case PLAY:
			break;
		case OPTIONS:
			break;
		case QUIT:
			break;
		}
		break;
	case WidgetEvent::ON_CLICKED:
		switch (data.ID)
		{
		case PLAY:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL_1);
			return;
		case OPTIONS:
			MenuManager::GetInstance()->ChangeToMenu(1);
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
