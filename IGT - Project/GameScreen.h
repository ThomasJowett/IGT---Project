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
	virtual void Update(float deltaTime, std::vector<SDL_Event> &events);
	
	std::vector<PlayerController*> GetPlayerControllers() { return mPlayerControllers; }
	std::vector<GameObject*> GetAllGameObjects();
	std::vector<GameObject*> GetAllGameObjectsWithTag(std::string tag);

	void AddGameObjects(std::vector<GameObject*> gameObjects);
	void AddGameObject(GameObject* gameObject);

	void DestroyGameOject(GameObject* gameObject);

	void RemoveGameObject(GameObject* gameObject);

	virtual void GameIsPaused(bool isGamePaused) {}

	void SortObjectsDepth(Camera* camera);

	bool ManageCameras();

	void SetUpCameras();

	Camera* GetCamera() { return &mCamera; }

	const char* GetScreenName() { return mScreenName; }

	std::vector<Collider*> GetAllColliders() { return mCollisionObejcts; }

protected:
	const char* mScreenName;

	SceneNode * Root;
	SceneNode * RootWidget;
	std::vector<SceneNode*>mGameObjects;
	std::vector<std::unique_ptr<GameObject>>mUIWidgets;
	std::vector<PlayerController*>mPlayerControllers;

	std::vector<Collider*> mCollisionObejcts;
	
	Shader* mShaderBasic;
	Shader* mShaderGUI;
	Shader* mPostProcessShader;
	Shader* mSplitScreenShader;

	FrameBuffer mFrameBuffer;
	FrameBuffer mCamera1FrameBuffer;
	FrameBuffer mCamera2FrameBuffer;

	Mesh* mFullscreenQuad;

	Camera mCamera;
	Camera mCamera1;
	Camera mCamera2;

private:
	GameObject* mCameraTarget1;
	GameObject* mCameraTarget2;

	std::vector<GameObject*> mToRemoveList;
};

#endif //_GAMESCREEN_H