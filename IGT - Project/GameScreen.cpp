#include "GameScreen.h"
#include <iostream>
#include "Settings.h"
#include "GeometryGenerator.h"
#include "Texture2D.h"
#include "Collision.h"
#include "PlayerPawn.h"

GameScreen::GameScreen()
{
	Root = new SceneNode();
	RootWidget = new SceneNode();
	mShaderBasic = new BasicShader();
	mShaderGUI = new GUIShader();
	mSplitScreenShader = new SplitScreenShader();

	mPostProcessShader = new NoPostProcessShader();

	Settings::GetInstance()->ApplySettings();

	mFrameBuffer.GenerateFBO(Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight());

	mCamera1FrameBuffer.GenerateFBO(Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight());
	mCamera2FrameBuffer.GenerateFBO(Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight());
	
	mFullscreenQuad = Geometry::CreateFullscreenQuad();
}


GameScreen::~GameScreen()
{
	mGameObjects.clear();
	mUIWidgets.clear();
	if (mShaderBasic) delete mShaderBasic;
	if (mShaderGUI) delete mShaderGUI;

	if (mSplitScreenShader) delete mSplitScreenShader;


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

	Matrix4x4 identityMatrix;

	if (mPlayerControllers.size() == 2 && ManageCameras())
	{
		mCamera1.UpdateView(mShaderBasic);
		mCamera1FrameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SortObjectsDepth(&mCamera1);
		Root->Traverse(mShaderBasic, identityMatrix);
		mCamera1FrameBuffer.Unbind();

		mCamera2.UpdateView(mShaderBasic);
		mCamera2FrameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SortObjectsDepth(&mCamera2);
		Root->Traverse(mShaderBasic, identityMatrix);
		mCamera2FrameBuffer.Unbind();

		mFrameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mSplitScreenShader->Bind();
		mSplitScreenShader->UpdateVector2(6, mCamera1.GetTransform()->mPosition);
		mSplitScreenShader->UpdateVector2(7, mCamera2.GetTransform()->mPosition);

		//Bind the render to texture as the texture to be drawn
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mCamera1FrameBuffer.GetColourTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mCamera2FrameBuffer.GetColourTexture());

		mFullscreenQuad->Draw();
		mFrameBuffer.Unbind();
	}
	else
	{

		mCamera.UpdateView(mShaderBasic);

		SortObjectsDepth(&mCamera);

		//Bind the render target to the off screen render target
		mFrameBuffer.Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Traverse through the scene graph rendering each object starting at the root
		Root->Traverse(mShaderBasic, identityMatrix);
		//Collision::GetQuadtree()->Render(mShaderBasic);
		mFrameBuffer.Unbind();
	}

	//Now render to the back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mPostProcessShader->Bind();
	mPostProcessShader->UpdateInteger(TINT_COLOUR_U, 5);
	mPostProcessShader->UpdateInteger(SCREEN_HEIGHT_U, Settings::GetInstance()->GetScreenHeight());
	mPostProcessShader->UpdateInteger(SCREEN_WIDTH_U, Settings::GetInstance()->GetScreenWidth());

	//Bind the render to texture as the texture to be drawn
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
	
	//Render the UI
	identityMatrix = Matrix4x4();
	RootWidget->Traverse(mShaderGUI, identityMatrix);

	glEnable(GL_DEPTH_TEST);
}

void GameScreen::Update(float deltaTime, std::vector<SDL_Event>& events)
{
	for (auto playerController : mPlayerControllers)
	{
		playerController->Update(events);
	}

	Root->Traverse(deltaTime);

	RootWidget->Traverse(deltaTime);

	for (GameObject* gameObject : mToRemoveList)
	{
		std::vector<Collider*> colliders = gameObject->GetAllComponents<Collider>();
		int collidersRemoved = 0;

		if (collidersRemoved < colliders.size())
		{
			for (int i = 0; i < mCollisionObejcts.size(); i++)
			{
				for (Collider* collider : colliders)
				{
					if (!collider->GeneratesOverlapEvents())
					{
						collidersRemoved++;
					}
					else if (mCollisionObejcts[i] == collider)
					{
						mCollisionObejcts.erase(mCollisionObejcts.begin() + i);
						collidersRemoved++;
					}
				}
			}
		}

		for(std::vector<SceneNode*> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{
			if (*it == gameObject)
			{
				

				mGameObjects.erase(it);

				if (gameObject->GetDeathFlag())
				{
					delete gameObject;
				}
				else
				{
					std::cout << "removed game object" << gameObject->GetUniqueID() << " " << mGameObjects.size() << "\n";
					gameObject->RemoveSelf();
				}
				
				break;
			}
		}
	}

	mToRemoveList.clear();
}

std::vector<GameObject*> GameScreen::GetAllGameObjects()
{
	std::vector<GameObject*> returnObjects;

	for (SceneNode* sceneNode : mGameObjects)
	{
		returnObjects.push_back(dynamic_cast<GameObject*>(sceneNode));
	}
	return returnObjects;
}

std::vector<GameObject*> GameScreen::GetAllGameObjectsWithTag(std::string tag)
{
	std::vector<GameObject*> returnObjects;

	for (SceneNode* sceneNode : mGameObjects)
	{
		if (GameObject* gameobject = dynamic_cast<GameObject*>(sceneNode))
		{
			if (gameobject->GetName() == tag)
				returnObjects.push_back(gameobject);
		}
	}
	return returnObjects;
}

//Add a list of game objects to the scene
void GameScreen::AddGameObjects(std::vector<GameObject*> gameObjects)
{
	for (GameObject* gameObject : gameObjects)
	{
		AddGameObject(gameObject);
	}
}

//Add a single game object to the scene
void GameScreen::AddGameObject(GameObject * gameObject)
{
	gameObject->AddAllChildrenToList(mGameObjects);

	std::vector<Collider*> colliders = gameObject->GetAllComponents<Collider>();
	for (Collider* collider : colliders)
	{
		if(collider->GeneratesOverlapEvents())
			mCollisionObejcts.push_back(collider);
	}

	if (gameObject->GetParent() == nullptr)
	{
		Root->AddChild(gameObject);
	}
}

void GameScreen::DestroyGameOject(GameObject * gameObject)
{
	gameObject->Destroy();
	mToRemoveList.emplace_back(gameObject);
}

void GameScreen::RemoveGameObject(GameObject * gameObject)
{
	gameObject->SetActive(false);
	mToRemoveList.emplace_back(gameObject);
}

void GameScreen::SortObjectsDepth(Camera* camera)
{
	for (SceneNode* sceneNode : mGameObjects)
	{
		if (sceneNode)
		{
			GameObject* gameObject = dynamic_cast<GameObject*>(sceneNode);
			if (gameObject->GetLayer() == SORTED)
			{
				Vector3D position = gameObject->GetLocalTransform()->mPosition;

				float topOfScreen = camera->GetTransform()->mPosition.y + (camera->GetOrthoHeight() / 2);
				float bottomofScreen = camera->GetTransform()->mPosition.y - (camera->GetOrthoHeight() / 2);

				if (position.y < topOfScreen && position.y > bottomofScreen)
				{
					float alpha = (position.y - camera->GetTransform()->mPosition.y + (camera->GetOrthoHeight() / 2)) / camera->GetOrthoHeight();
					float neardepth = camera->GetTransform()->mPosition.z - camera->GetNearDepth() - 1.0f;
					float fardepth = camera->GetTransform()->mPosition.z - camera->GetFarDepth() + 1.0f;

					float objectDepth = neardepth + alpha * (fardepth - neardepth);

					gameObject->GetLocalTransform()->mPosition.z = objectDepth;
				}
			}
		}
	}
}

bool GameScreen::ManageCameras()
{
	if (!mCameraTarget1 || !mCameraTarget2)
		return false;

	Vector2D player1Location = mCameraTarget1->GetLocalTransform()->mPosition;

	Vector2D player2Location = mCameraTarget2->GetLocalTransform()->mPosition;

	Vector2D MidPointLocation = (player1Location + player2Location) / 2;

	float screenwidth = Settings::GetInstance()->GetScreenWidth() / 14.0f;
	float screenHeight = Settings::GetInstance()->GetScreenHeight() / 10.0f;

	float offset = 10.0f;

	if (Vector2D::Distance(player1Location, player2Location) > screenwidth)
	{
		player1Location += (screenwidth / 2) * ((MidPointLocation - player1Location).GetNormalized());
		player1Location.y += offset;
		//player1Location.x += (screenwidth/2) * ((MidPointLocation - player1Location).GetNormalized().x);
		//player1Location.y += (screenHeight / 2) * ((MidPointLocation - player1Location).GetNormalized().y);
		mCamera1.GetTransform()->mPosition.x = player1Location.x;
		mCamera1.GetTransform()->mPosition.y = player1Location.y;

		player2Location += (screenwidth / 2) * ((MidPointLocation - player2Location).GetNormalized());
		player2Location.y += offset;
		//player2Location.x += (screenwidth / 2) * ((MidPointLocation - player2Location).GetNormalized().x);
		//player2Location.y += (screenHeight / 2) * ((MidPointLocation - player2Location).GetNormalized().y);
		mCamera2.GetTransform()->mPosition.x = player2Location.x;
		mCamera2.GetTransform()->mPosition.y = player2Location.y;
		return true;
	}
	else
	{
		mCamera.GetTransform()->mPosition.x = MidPointLocation.x;
		mCamera.GetTransform()->mPosition.y = MidPointLocation.y + offset;
		return false;
	}
}

void GameScreen::SetUpCameras()
{
	PlayerPawn* player1pawn = dynamic_cast<PlayerPawn*>(mPlayerControllers[0]->GetPossessedPawn());

	PlayerPawn* player2pawn = dynamic_cast<PlayerPawn*>(mPlayerControllers[1]->GetPossessedPawn());

	if (!player1pawn || !player2pawn)
		return;

	mCameraTarget1 = player1pawn->GetPossessedCharacter();
	mCameraTarget2 = player2pawn->GetPossessedCharacter();

	if (!mCameraTarget1 || !mCameraTarget2)
		return;

}
