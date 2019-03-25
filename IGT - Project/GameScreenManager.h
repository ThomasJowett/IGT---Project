#ifndef _GAMESCREENMANAGER_H
#define _GAMESCREENMANAGER_H

#include <SDL.h>
#include <vector>

#include "Commons.h"
#include "GameScreen.h"

class GameScreen;

class GameScreenManager
{
public:
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, std::vector<SDL_Event>& events);

	void ChangeScreen(SCREENS newScreen);

	static GameScreenManager * GetInstance();

	GameScreen* GetCurrentScreen()const { return mCurrentScreen; }

	void PauseGame() { mPaused = true; GetCurrentScreen()->GameIsPaused(mPaused);}
	void ResumeGame() { mPaused = false; GetCurrentScreen()->GameIsPaused(mPaused);}

	bool TogglePause() { mPaused = !mPaused; GetCurrentScreen()->GameIsPaused(mPaused); return mPaused; }

	bool IsGamePaused() { return mPaused; }

private:
	GameScreenManager(SCREENS startScreen);

	GameScreen* mCurrentScreen;

	SCREENS mScreenToChange;

	bool mDelayedScreenChange = false;

	void DelayedScreenChange(SCREENS newScreen);

	bool mPaused = false;

	int mNumPlayers = 1;
};

#endif //_GAMESCREENMANAGER_H