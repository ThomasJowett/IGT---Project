#include "GameScreen.h"
#include <iostream>
#include "Settings.h"

GameScreen::GameScreen()
{
	Root = new SceneNode();
	RootWidget = new SceneNode();
	mShaderBasic = new BasicShader();
	mShaderGUI = new GUIShader();

	Settings::GetInstance()->SetCamera(&mCamera);

	Settings::GetInstance()->ApplySettings();
}


GameScreen::~GameScreen()
{
	mGameObjects.clear();
	mUIWidgets.clear();
	if (mShaderBasic) delete mShaderBasic;
	if (mShaderGUI) delete mShaderGUI;

	for (std::vector< PlayerController* >::iterator it = mPlayerControllers.begin(); it != mPlayerControllers.end(); ++it)
	{
		delete (*it);
		*it = nullptr;
	}

	delete Root;
	delete RootWidget;
}

void GameScreen::Render()
{
	//Render Objects
	mShaderBasic->Bind();
	mCamera.UpdateView(mShaderBasic);

	Matrix4x4 identityMatrix;

	Root->Traverse(mShaderBasic, identityMatrix);

	//Render UI Widgets
	mShaderGUI->Bind();
	glDisable(GL_DEPTH_TEST);
	mCamera.UpdateView(mShaderGUI);
	
	identityMatrix = Matrix4x4();
	RootWidget->Traverse(mShaderGUI, identityMatrix);

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

std::vector<GameObject*> GameScreen::GetAllGameObjects()
{
	std::vector<GameObject*> returnObjects;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		returnObjects.push_back(it->get());
	}
	return returnObjects;
}

std::vector<GameObject*> GameScreen::GetAllGameObjectsWithTag(std::string tag)
{
	std::vector<GameObject*> returnObjects;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetName() == tag)
			returnObjects.push_back(it->get());
	}
	return returnObjects;
}
