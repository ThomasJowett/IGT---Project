#include "MainMenu.h"
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "Image.h"

MainMenu::MainMenu(Transform* transform)
	:UIMenu("MainMenu", transform)
{
	CreateWidgets();

	//TODO: add the button prompts (A and B) to screen somewhere
}


MainMenu::~MainMenu()
{
	
}

void MainMenu::CreateWidgets()
{
	Image* image = new Image("Logo", { 0.5, 0.25 }, { 0,0 }, Texture2D::GetTexture2D("Images/Logo.png"), { 90, 73 });
	mWidgets.emplace_back(image);
	AddChild(image);

	image = new Image("A ButtonPrompt", { 1,1 }, { -65, 20 }, Texture2D::GetTexture2D("Images/Button_A.png"), { 16,16 });
	mWidgets.emplace_back(image);
	AddChild(image);

	GLuint ButtonTexture = Texture2D::GetTexture2D("Images/ButtonsMenu.png");

	Button* button = new Button(ButtonTexture, { 0.5,0.6f }, { 0, 25 }, { 100, 20 }, "Play");
	button->mWidgetData.ID = PLAY;
	button->AddObserver(this);
	mWidgets.emplace_back(button);
	AddChild(button);

	button = new Button(ButtonTexture, { 0.5,0.6f }, { 0,0 }, { 100, 20 }, "Options");
	button->mWidgetData.ID = OPTIONS;
	button->AddObserver(this);
	mWidgets.emplace_back(button);
	AddChild(button);

	button = new Button(ButtonTexture, { 0.5,0.6f }, { 0,-25 }, { 100, 20 }, "Quit");
	button->mWidgetData.ID = QUIT;
	button->AddObserver(this);
	mWidgets.emplace_back(button);
	AddChild(button);
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
			GameScreenManager::GetInstance()->ResumeGame();
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
