#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenMenu.h"
#include "GameScreenLevel1.h"

static GameScreenManager* instance = 0;

GameScreenManager::GameScreenManager(SCREENS startScreen)
{
	instance = this;
	mCurrentScreen = nullptr;

	//Ensure the first screen is set up.
	ChangeScreen(startScreen);
}

void GameScreenManager::DelayedScreenChange(SCREENS newScreen)
{
	//Initialise the new screen.
	switch (newScreen)
	{
	case SCREEN_INTRO:
		break;

	case SCREEN_MENU:
		mCurrentScreen = new GameScreenMenu();
		break;
	case SCREEN_LEVEL_1:
		mCurrentScreen = new GameScreenLevel1();
		break;
	default:
		mCurrentScreen = new GameScreenMenu();
		break;
	}
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

	//Screen is finished updateing so can be deleted
	if (mDelayedScreenChange)
	{
		delete mCurrentScreen;
		mDelayedScreenChange = false;
		DelayedScreenChange(mScreenToChange);
	}
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	//Let the old screen finish its update
	if (mCurrentScreen)
	{
		mDelayedScreenChange = true;
		mScreenToChange = newScreen;
	}
	else
	{
		DelayedScreenChange(newScreen);
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
