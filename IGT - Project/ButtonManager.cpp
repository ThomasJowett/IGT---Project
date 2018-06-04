#include "ButtonManager.h"
#include "GameScreenManager.h"



ButtonManager::ButtonManager()
{
}


ButtonManager::~ButtonManager()
{
}

void ButtonManager::OnNotify(ButtonEvent event, int ID)
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
		case 0:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL_1);
			return;
		case 1:
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_OPTIONS);
			return;
		case 2:
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
