#include "GameScreen.h"



GameScreen::GameScreen()
{
	//Root = new SceneNode();
}


GameScreen::~GameScreen()
{
	for (std::vector< GameObject* >::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		delete (*it);
		*it = nullptr;
	}
	mGameObjects.clear();

}

void GameScreen::Render()
{
	mShader->Bind();
	mCamera.Update(mShader);
	//Root->Traverse();
}

void GameScreen::Update(float deltaTime, std::vector<SDL_Event> events)
{
	
	for (auto playerController : mPlayerControllers)
	{
		playerController->Update(events);
	}
	
	for (auto gameObject : mGameObjects)
	{
		gameObject->Update(deltaTime);
	}
}
