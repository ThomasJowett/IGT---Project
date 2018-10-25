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


GameScreenLevel1::GameScreenLevel1()
{
	mShaderBasic = new BasicShader();

	mCamera.GetTransform()->mPosition = Vector3D(0, 0, 100);

	GLuint goblinTexture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint batTexture = Texture2D::LoadTexture2D("SpriteSheets/rat and bat spritesheet calciumtrice.png");
	GLuint SnakeTexture = Texture2D::LoadTexture2D("SpriteSheets/snake spritesheet calciumtrice.png");
	GLuint circleTexture = Texture2D::LoadTexture2D("Images/Circle.png");
	GLuint squareTexture = Texture2D::LoadTexture2D("Images/Square.png");
	GLuint CursorTexture = Texture2D::LoadTexture2D("Images/Cursor_Default.png");

	Transform* transform;
	GameObject* gameObject;

	PhysicsMaterial physicsMaterial = { 30, 0.8, 0.5, 10 };
	PhysicsMaterial physicsMaterialcircle = { 3, 0.8, 0.5, 1 };

	//Cursor
	transform = new Transform();
	GameObject* cursor = new GameObject("Cursor", transform, false);
	cursor->AddComponent<Sprite>(CursorTexture, 16, 16);

	MainMenuPawn* menu = new MainMenuPawn(cursor);

	//player 1
	transform = new Transform(Vector3D(200, -200, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 1", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 4, 10);
	gameObject->AddComponent<Sprite>(squareTexture, 20, 10, Vector2D(0, -20));
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 1", { 0,0,0 }, 0, 20, CENTER);
	gameObject->AddComponent<Box2D>(20, 10, Vector2D(0, -20));
	gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<AnimatorCharacter>();
	mGameObjects.emplace_back(gameObject);
	PlayerPawn* characterController = new PlayerPawn(gameObject, menu);

	//player 2
	transform = new Transform(Vector3D(100, -100, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 2", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 4, 10);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 2", { 0,0,0 }, 0, 20, CENTER);
	gameObject->AddComponent<Box2D>(18, 30, Vector2D(0, -10));
	gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<AnimatorCharacter>();
	mGameObjects.emplace_back(gameObject);
	PlayerPawn* character2Controller = new PlayerPawn(gameObject, menu);

	//temporary spawning enemy
	//transform = new Transform(Vector3D(-10, 0, 0), 0, Vector2D(1, 1));
	//gameObject = new GameObject("Snake", transform);
	//gameObject->AddComponent<RigidBody2D>(10, Vector2D(0, 0), 10, 0, physicsMaterial);
	//gameObject->AddComponent<Circle2D>(8, Vector2D());
	//gameObject->AddComponent<Sprite>(SnakeTexture, 32, 32, 10, 5);
	//gameObject->AddComponent<AnimatorSnake>();
	//
	//mGameObjects.emplace_back(gameObject);

	//Pause Menu
	UIMenu* pauseMenu = new PauseMenu(new Transform(), characterController);
	mUIWidgets.emplace_back(pauseMenu);
	MenuManager::GetInstance()->AddMenu(pauseMenu);
	
	mUIWidgets.emplace_back(cursor);

	//PlayerControllers
	PlayerController* playerController = new PlayerController(0, characterController);
	mPlayerControllers.push_back(playerController);

	PlayerController* playerController2 = new PlayerController(1, character2Controller);
	mPlayerControllers.push_back(playerController2);

	
	
	transform = new Transform(Vector3D(0, 0, 5), 0, Vector2D(1, 1));
	gameObject = new GameObject("Circle", transform);
	gameObject->AddComponent<Sprite>(circleTexture, 32, 32);
	gameObject->AddComponent<Circle2D>(16, Vector2D());
	mGameObjects.emplace_back(gameObject);

	transform = new Transform(Vector3D(-80, 0, 5), 0, Vector2D(1, 1));
	gameObject = new GameObject("Square", transform);
	gameObject->AddComponent<Sprite>(squareTexture, 32, 32);
	gameObject->AddComponent<Box2D>(32, 32, Vector2D());
	mGameObjects.emplace_back(gameObject);

	transform = new Transform(Vector3D(0, -50, 5), 0, Vector2D(1, 1));
	gameObject = new GameObject("Ball", transform);
	gameObject->AddComponent<Sprite>(circleTexture, 16, 16);
	gameObject->AddComponent<Circle2D>(8, Vector2D());
	gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, physicsMaterialcircle);
	mGameObjects.emplace_back(gameObject);

	for (int i = 0; i < 10; i++)
	{
		gameObject = new GameObject(*gameObject);
		gameObject->GetTransform()->mPosition = Vector3D(800 * (float)rand() / (RAND_MAX)-400, 80 * (float)rand() / (RAND_MAX)+40, 5);
		//gameObject->GetComponent<RigidBody2D>()->SetVelocity(Vector2D(80 * (float)rand() / (RAND_MAX)-40, 80 * (float)rand() / (RAND_MAX)-40));
		mGameObjects.emplace_back(gameObject);
	}

	transform = new Transform(Vector3D(0, -50, 5), 0, Vector2D(1, 1));
	gameObject = new GameObject("little box", transform);
	gameObject->AddComponent<Sprite>(squareTexture, 16, 16);
	gameObject->AddComponent<Box2D>(16, 16, Vector2D());
	gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, physicsMaterialcircle);
	mGameObjects.emplace_back(gameObject);

	mTileMap = new TileMap("Maps/TestMapWithCollision.xml", "Maps/DungeonTileSet.png");
	mGameObjects.emplace_back(mTileMap);
}

GameScreenLevel1::~GameScreenLevel1()
{
	MenuManager::GetInstance()->RemoveAllMenus();
}

void GameScreenLevel1::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);

	std::vector<GameObject*> collisionObejcts;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetComponent<Collider>())
			collisionObejcts.push_back(it->get());
	}

	Collision::DetectCollisions(collisionObejcts);

	Collision::DetectCollisions(mTileMap, collisionObejcts);

	mCamera.GetTransform()->mPosition = mGameObjects[0]->GetTransform()->mPosition + Vector3D(0,0,100);
}