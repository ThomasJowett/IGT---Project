#ifndef _GAMESCREENMENU_H
#define _GAMESCREENMENU_H


#include "GameScreen.h"
#include "TextRender.h"
#include "Shader.h"
#include "Camera.h"

class GameScreenMenu : public GameScreen
{
public:
	GameScreenMenu();
	~GameScreenMenu();

	virtual void Render();
	virtual void Update(float deltaTime, std::vector<SDL_Event> e);
private:
	int texID;
	BasicShader* mShader;
	Camera camera;
};

#endif // !_GAMESCREENMENU_H