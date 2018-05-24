#include "GameScreen.h"



GameScreen::GameScreen()
{
	//Root = new SceneNode();
}


GameScreen::~GameScreen()
{
	mGameObjects.clear();
}

void GameScreen::Render()
{
	//Root->Traverse();
}

void GameScreen::Update(float deltaTime, std::vector<SDL_Event> events)
{
	/*
	for (auto playerController : mPlayerControllers)
	{
		playerController->Update(deltaTime, e);
	}
	*/
	for (auto gameObject : mGameObjects)
	{
		gameObject->Update(deltaTime);
	}
}
