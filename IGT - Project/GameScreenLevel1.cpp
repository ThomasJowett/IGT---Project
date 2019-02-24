#include "GameScreenLevel1.h"
#include "Settings.h"
#include "GameScreenManager.h"
#include "PlayerPawn.h"
#include "MainMenuPawn.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Collider.h"
#include "Collision.h"
#include "TextRender.h"
#include "RigidBody2D.h"
#include "AnimatorCharacter.h"
#include "AnimatorSnake.h"
#include "PauseMenu.h"
#include "Health.h"
#include "Attack.h"
#include "HUD.h"
#include "AIController.h"
#include "CameraFollow.h"
#include "RandomGenerator.h"
#include "SpawnManager.h"

#include "Prefab.h"


GameScreenLevel1::GameScreenLevel1()
{
	//Setup the blur on pause post process
	mShaderBlur = new BlurShader();
	mNoPostProcess = new NoPostProcessShader();

	mPostProcessShader = mNoPostProcess;

	GLuint SnakeTexture = Texture2D::GetTexture2D("SpriteSheets/snake spritesheet calciumtrice.png");


	Transform* transform;
	GameObject* gameObject;

	PhysicsMaterial physicsMaterial = { 30.0f, 0.8f, 0.5f, 10.0f };
	PhysicsMaterial physicsMaterialcircle = { 300.0f, 0.8f, 0.5f, 1.0f };

	MainMenuPawn* menu = new MainMenuPawn();

	mTileMap = new TileMap("TestMap.tmx");
	AddGameObject(mTileMap);

	mCamera.GetTransform()->mPosition = mTileMap->GetPlayerStart(0).to_Vector3D();

	Astar::Generator::GetInstance()->SetTileMap(mTileMap);

	//player 1
	gameObject = BarbarianCharacterPrefab().GetPrefab();
	gameObject->GetTransform()->mPosition = mTileMap->GetPlayerStart(0).to_Vector3D();
	AddGameObject(gameObject);
	PlayerPawn* character1 = new PlayerPawn(gameObject, menu);

	////player 2
	gameObject = ArcherCharacterPrefab().GetPrefab();
	gameObject->GetTransform()->mPosition = mTileMap->GetPlayerStart(1).to_Vector3D();
	AddGameObject(gameObject);
	PlayerPawn* character2 = new PlayerPawn(gameObject, menu);

	SpawnManager::SpawnGameObjects(mTileMap->GetSpawnRooms(), this);
	

	//temporary spawning enemy
	transform = new Transform(Vector3D(240, -1424, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Snake", transform);
	gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<Circle2D>(8, Vector2D());
	gameObject->AddComponent<Sprite>(SnakeTexture, 32, 32, 10, 5, Vector2D(0,16));
	gameObject->AddComponent<Attack>(25.0f, 2.0f);
	gameObject->AddComponent<Health>(20.0f);
	gameObject->AddComponent<AnimatorSnake>();
	gameObject->AddComponent<AIController>();
	AddGameObject(gameObject);

	//Pause Menu
	UIMenu* pauseMenu = new PauseMenu(new Transform(), character1);
	mUIWidgets.emplace_back(pauseMenu);
	RootWidget->AddChild(pauseMenu);
	MenuManager::GetInstance()->AddMenu(pauseMenu);

	pauseMenu = new HUD();
	mUIWidgets.emplace_back(pauseMenu);
	RootWidget->AddChild(pauseMenu);
	MenuManager::GetInstance()->AddMenu(pauseMenu);
	

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

void GameScreenLevel1::Update(float deltaTime, std::vector<SDL_Event> events)
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
