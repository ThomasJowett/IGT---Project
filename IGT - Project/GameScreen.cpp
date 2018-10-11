#include "GameScreen.h"
#include <iostream>
#include "Settings.h"

GameScreen::GameScreen()
{
	//Root = new SceneNode();
	mShaderBasic = new BasicShader();
	mShaderGUI = new GUIShader();

	Settings::GetInstance()->SetCamera(&mCamera);

	Settings::GetInstance()->ApplySettings();

	//mShaderBasic->UpdateMatrixUniform(PROJECTION_U, mCamera.GetProjection());
	//mShaderGUI->UpdateMatrixUniform(PROJECTION_U, mCamera.GetProjection());
}


GameScreen::~GameScreen()
{
	mGameObjects.clear();
	if (mShaderBasic) delete mShaderBasic;

	for (std::vector< PlayerController* >::iterator it = mPlayerControllers.begin(); it != mPlayerControllers.end(); ++it)
	{
		delete (*it);
		*it = nullptr;
	}
}

void GameScreen::Render()
{
	mShaderBasic->Bind();
	mCamera.UpdateView(mShaderBasic);

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetActive())
			it->get()->Render(mShaderBasic);
	}

	//Root->Traverse();

	mShaderGUI->Bind();
	glDisable(GL_DEPTH_TEST);
	mCamera.UpdateView(mShaderGUI);
	
	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mUIWidgets.begin(); it != mUIWidgets.end(); ++it)
	{
		if (it->get()->GetActive())
			it->get()->Render(mShaderGUI);
	}

	mFPS.Render(mShaderGUI);

	glEnable(GL_DEPTH_TEST);
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

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mUIWidgets.begin(); it != mUIWidgets.end(); ++it)
	{
		if (it->get()->GetActive())
			it->get()->Update(deltaTime);
	}

	mFPS.Update(deltaTime);
}
