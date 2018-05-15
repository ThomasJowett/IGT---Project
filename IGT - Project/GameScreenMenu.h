#ifndef _GAMESCREENMENU_H
#define _GAMESCREENMENU_H


#include "GameScreen.h"
#include "TextRender.h"

class GameScreenMenu : public GameScreen
{
public:
	GameScreenMenu();
	~GameScreenMenu();

	virtual void Render();
	virtual void Update(float deltaTime, std::vector<SDL_Event> e);
private:
	TextRender* mTextRender;
};

#endif // !_GAMESCREENMENU_H