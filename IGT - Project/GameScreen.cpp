#include "GameScreen.h"
#include <iostream>
#include "Settings.h"
#include "GeometryGenerator.h"
#include "Texture2D.h"

GameScreen::GameScreen()
{
	Root = new SceneNode();
	RootWidget = new SceneNode();
	mShaderBasic = new BasicShader();
	mShaderGUI = new GUIShader();
	mShaderBlur = new BlurShader();

	Settings::GetInstance()->SetCamera(&mCamera);

	Settings::GetInstance()->ApplySettings();

	mFrameBuffer.GenerateFBO(Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight());
	Settings::GetInstance()->AddObserver(&mFrameBuffer);
	mFullscreenQuad = Geometry::CreateFullscreenQuad();
}


GameScreen::~GameScreen()
{
	mGameObjects.clear();
	mUIWidgets.clear();
	if (mShaderBasic) delete mShaderBasic;
	if (mShaderGUI) delete mShaderGUI;
	if (mShaderBlur) delete mShaderBlur;

	for (std::vector< PlayerController* >::iterator it = mPlayerControllers.begin(); it != mPlayerControllers.end(); ++it)
	{
		delete (*it);
		*it = nullptr;
	}

	delete Root;
	delete RootWidget;

	Settings::GetInstance()->RemoveObserver(&mFrameBuffer);
}

void GameScreen::Render()
{
	//Render Objects
	mShaderBasic->Bind();
	mCamera.UpdateView(mShaderBasic);

	mFrameBuffer.Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Matrix4x4 identityMatrix;

	Root->Traverse(mShaderBasic, identityMatrix);
	mFrameBuffer.Unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mShaderBlur->Bind();
	mShaderBlur->UpdateInteger(SCREEN_HEIGHT_U, Settings::GetInstance()->GetScreenHeight());
	mShaderBlur->UpdateInteger(SCREEN_WIDTH_U, Settings::GetInstance()->GetScreenWidth());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFrameBuffer.GetColourTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mFrameBuffer.GetDepthTexture());
	mFullscreenQuad->Draw();

	glActiveTexture(GL_TEXTURE0);
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
