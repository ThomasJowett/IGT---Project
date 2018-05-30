#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H

#include <SDL.h>
#include <vector>
#include "GameObject.h"
#include "Shader.h"
#include "Camera.h"
#include "PlayerController.h"

class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, std::vector<SDL_Event> e);
	//std::vector<GameObject*> GetAllGameObjectsWithTag(std::string tag)const;

protected:
	//SceneNode * Root;
	std::vector<GameObject*>mGameObjects;
	std::vector<PlayerController*>mPlayerControllers;

	Shader* mShader;
	Camera mCamera;
};

#endif //_GAMESCREEN_H