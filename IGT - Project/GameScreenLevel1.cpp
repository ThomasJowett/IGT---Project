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



	for (int i = 0; i < 5; i++)
	{
		//AddGameObjects(BarrelPrefab().GetPrefab());
		//mGameObjects.back()->GetTransform()->mPosition = Vector3D(Random::FloatInRange(300, 700), Random::FloatInRange(-450, -800), 0);
		//
		//AddGameObjects(ChestPrefab().GetPrefab());
		//mGameObjects.back()->GetTransform()->mPosition = Vector3D(Random::FloatInRange(300, 700), Random::FloatInRange(-450, -800), 0);
		//
		//AddGameObjects(LargeLootPrefab().GetPrefab());
		//mGameObjects.back()->GetTransform()->mPosition = Vector3D(Random::FloatInRange(300, 700), Random::FloatInRange(-450, -800), 0);
		//
		//AddGameObjects(MediumLootPrefab().GetPrefab());
		//mGameObjects.back()->GetTransform()->mPosition = Vector3D(Random::FloatInRange(300, 700), Random::FloatInRange(-450, -800), 0);

		
	}

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
	
	
	//
	//transform = new Transform(Vector3D(0, -50, 5), 0, Vector2D(1, 1));
	//gameObject = new GameObject("little box", transform);
	//gameObject->AddComponent<Sprite>(squareTexture, 16, 16);
	//gameObject->AddComponent<Box2D>(16, 16, Vector2D());
	//gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, physicsMaterialcircle);
	//mGameObjects.emplace_back(gameObject);
	//Root->AddChild(gameObject);

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

	std::vector<GameObject*> collisionObejcts;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		Collider* collider = it->get()->GetComponent<Collider>();
		if (collider && it->get()->GetActive())
		{
			collider->ClearTestedCollisionWith();
			collisionObejcts.push_back(it->get());
		}
	}

	mShaderBasic->Bind();
	Collision::DetectCollisions(collisionObejcts);

	Collision::DetectCollisions(mTileMap, collisionObejcts);
}

void GameScreenLevel1::GameIsPaused(bool isGamePaused)
{
	mPostProcessShader = (isGamePaused) ? mShaderBlur : mNoPostProcess;
}
