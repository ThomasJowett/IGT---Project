#ifndef _GAMESCREENMENU_H
#define _GAMESCREENMENU_H


#include "GameScreen.h"
#include "TextRender.h"
#include "Shader.h"
#include "Camera.h"
#include "Button.h"
#include "ButtonManager.h"

class GameScreenMenu : public GameScreen
{
public:
	GameScreenMenu();
	~GameScreenMenu();

	virtual void Render()override;
	virtual void Update(float deltaTime, std::vector<SDL_Event> e)override;
private:

	std::vector<Button*> mButtons;
	ButtonManager* mMenuButtonsManager;
};

#endif // !_GAMESCREENMENU_H