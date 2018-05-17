#ifndef _GAMESCREENMANAGER_H
#define _GAMESCREENMANAGER_H

#include <SDL.h>
#include <vector>

#include "Commons.h"

class GameScreen;

class GameScreenManager
{
public:
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, std::vector<SDL_Event> events);

	void ChangeScreen(SCREENS newScreen);

	static GameScreenManager * GetInstance();

	GameScreen* GetCurrentScreen()const { return mCurrentScreen; }

private:
	GameScreenManager(SCREENS startScreen);

	GameScreen* mCurrentScreen;
};

#endif //_GAMESCREENMANAGER_H