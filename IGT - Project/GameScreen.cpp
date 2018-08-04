#include "GameScreen.h"
#include <iostream>
#include "Settings.h"

GameScreen::GameScreen()
{
	//Root = new SceneNode();
	mShader = nullptr;

	Settings::GetInstance()->SetCamera(&mCamera);

	//mCamera.Orthographic((480), (270), 0, 1000);

	Settings::GetInstance()->ApplySettings();
}


GameScreen::~GameScreen()
{
	mGameObjects.clear();
	if (mShader) delete mShader;

	for (std::vector< PlayerController* >::iterator it = mPlayerControllers.begin(); it != mPlayerControllers.end(); ++it)
	{
		delete (*it);
		*it = nullptr;
	}
}

void GameScreen::Render()
{
	mShader->Bind();
	mCamera.Update(mShader);

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetActive())
			it->get()->Render(mShader);
	}

	//Root->Traverse();
}

void GameScreen::Update(float deltaTime, std::vector<SDL_Event> events)
{
	for (auto playerController : mPlayerControllers)
	{
		playerController->Update(events);
	}
	
	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetActive())
			it->get()->Update(deltaTime);
	}
}
