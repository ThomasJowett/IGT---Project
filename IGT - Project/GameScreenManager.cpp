#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenMenu.h"

static GameScreenManager* instance = 0;

GameScreenManager::GameScreenManager(SCREENS startScreen)
{
	instance = this;
	mCurrentScreen = NULL;

	//Ensure the first screen is set up.
	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	delete mCurrentScreen;
	mCurrentScreen = nullptr;
}

void GameScreenManager::Render()
{
	mCurrentScreen->Render();
}

void GameScreenManager::Update(float deltaTime, std::vector<SDL_Event> events)
{
	mCurrentScreen->Update(deltaTime, events);
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	//Clear up the old screen.
	if (mCurrentScreen != NULL)
	{
		delete mCurrentScreen;
	}

	//Initialise the new screen.
	switch (newScreen)
	{
	case SCREEN_INTRO:
		break;

	case SCREEN_MENU:
		mCurrentScreen = new GameScreenMenu();
		break;
	default:
		break;
	}
}

GameScreenManager * GameScreenManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new GameScreenManager(SCREEN_MENU);
	}
	return instance;
}
