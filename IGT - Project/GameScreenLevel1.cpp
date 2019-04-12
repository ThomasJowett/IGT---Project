#include "GameScreenLevel1.h"
#include "Settings.h"
#include "GameScreenManager.h"
#include "PlayerPawn.h"
#include "MainMenuPawn.h"
#include "Collider.h"
#include "Collision.h"
#include "PauseMenu.h"
#include "HUD.h"
#include "Astar.h"
#include "RandomGenerator.h"
#include "SpawnManager.h"

#include "Prefab.h"


GameScreenLevel1::GameScreenLevel1()
{
	mScreenName = "Level 1";
	//Setup the blur on pause post process
	mShaderBlur = new BlurShader();
	mNoPostProcess = new NoPostProcessShader();

	mPostProcessShader = mNoPostProcess;

	GameObject* gameObject;

	PhysicsMaterial physicsMaterial = { 30.0f, 0.8f, 0.5f, 10.0f };
	PhysicsMaterial physicsMaterialcircle = { 300.0f, 0.8f, 0.5f, 1.0f };

	MainMenuPawn* menuPawn = new MainMenuPawn();

	mTileMap = new TileMap("TestMap.tmx");
	AddGameObject(mTileMap);

	mCamera.GetTransform()->mPosition = mTileMap->GetPlayerStart(0).to_Vector3D();

	Astar::Generator::GetInstance()->SetTileMap(mTileMap);

	//player 1
	gameObject = new GameObject(*Factory<Prefab>::CreateInstance("BarbarianCharacter")->GetPrefab());
	gameObject->GetLocalTransform()->mPosition = mTileMap->GetPlayerStart(0).to_Vector3D();
	AddGameObject(gameObject);
	PlayerPawn* character1 = new PlayerPawn(gameObject, menuPawn);

	//Pause Menu
	UIMenu* uiMenu = new PauseMenu(new Transform(), character1);
	mUIWidgets.emplace_back(uiMenu);
	RootWidget->AddChild(uiMenu);
	MenuManager::GetInstance()->AddMenu(uiMenu);

	//Player 1 HUD
	uiMenu = new Player1HUD(gameObject);
	mUIWidgets.emplace_back(uiMenu);
	RootWidget->AddChild(uiMenu);
	MenuManager::GetInstance()->AddMenu(uiMenu);

	//player 2
	gameObject = new GameObject(*ArcherCharacterPrefab().GetPrefab());
	gameObject->GetLocalTransform()->mPosition = mTileMap->GetPlayerStart(1).to_Vector3D();
	AddGameObject(gameObject);
	PlayerPawn* character2 = new PlayerPawn(gameObject, menuPawn);

	//Player 2 HUD
	uiMenu = new Player2HUD(gameObject);
	mUIWidgets.emplace_back(uiMenu);
	RootWidget->AddChild(uiMenu);
	MenuManager::GetInstance()->AddMenu(uiMenu);

	SpawnManager::SpawnGameObjects(mTileMap->GetSpawnRooms(), this);

	//PlayerControllers
	PlayerController* playerController = new PlayerController(0, character1);
	mPlayerControllers.push_back(playerController);

	PlayerController* playerController2 = new PlayerController(1, character2);
	mPlayerControllers.push_back(playerController2);

	SetUpCameras();
}

GameScreenLevel1::~GameScreenLevel1()
{
	MenuManager::GetInstance()->RemoveAllMenus();

	if (mShaderBlur) delete mShaderBlur;
	if (mNoPostProcess) delete mNoPostProcess;
}

void GameScreenLevel1::Update(float deltaTime, std::vector<SDL_Event> &events)
{
	GameScreen::Update(deltaTime, events);

	std::vector<Collider*> activeColliders;

	for (Collider* collider: mCollisionObejcts)
	{
		if (collider->GetParent()->GetActive())
		{
			activeColliders.push_back(collider);
			collider->ClearTestedCollisionWith();
		}
	}

	Collision::DetectCollisions(activeColliders);

	Collision::DetectCollisions(mTileMap, activeColliders);
}

void GameScreenLevel1::GameIsPaused(bool isGamePaused)
{
	mPostProcessShader = (isGamePaused) ? mShaderBlur : mNoPostProcess;
}
