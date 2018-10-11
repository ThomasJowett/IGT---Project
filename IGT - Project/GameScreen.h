#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H

#include <SDL.h>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "UIWidget.h"
#include "Shader.h"
#include "Camera.h"
#include "PlayerController.h"
#include "FPS.h"

class GameScreen
{
public:
	GameScreen();
	virtual ~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, std::vector<SDL_Event> events);
	//std::vector<GameObject*> GetAllGameObjectsWithTag(std::string tag)const;
	std::vector<PlayerController*> GetPlayerControllers() { return mPlayerControllers; }

protected:
	//SceneNode * Root;
	std::vector<std::unique_ptr<GameObject>>mGameObjects;
	std::vector<std::unique_ptr<GameObject>>mUIWidgets;
	std::vector<PlayerController*>mPlayerControllers;

	
	Shader* mShaderBasic;
	Shader* mShaderGUI;

	Camera mCamera;

	FPS mFPS;
};

#endif //_GAMESCREEN_H