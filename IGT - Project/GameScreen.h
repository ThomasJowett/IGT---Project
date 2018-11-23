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
#include "PostProcess.h"

class GameScreen
{
public:
	GameScreen();
	virtual ~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, std::vector<SDL_Event> events);
	
	std::vector<PlayerController*> GetPlayerControllers() { return mPlayerControllers; }
	std::vector<GameObject*> GetAllGameObjects();
	std::vector<GameObject*> GetAllGameObjectsWithTag(std::string tag);

	void AddGameObjects(std::vector<GameObject*> gameObjects);

protected:
	SceneNode * Root;
	SceneNode * RootWidget;
	std::vector<std::unique_ptr<GameObject>>mGameObjects;
	std::vector<std::unique_ptr<GameObject>>mUIWidgets;
	std::vector<PlayerController*>mPlayerControllers;

	
	Shader* mShaderBasic;
	Shader* mShaderGUI;
	Shader* mShaderBlur;

	FrameBuffer mFrameBuffer;

	Mesh* mFullscreenQuad;

	Camera mCamera;

	FPS mFPS;
};

#endif //_GAMESCREEN_H