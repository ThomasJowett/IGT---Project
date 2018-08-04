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
	mShader = new BasicShader();

	mCamera.GetTransform()->mPosition = Vector3D(0, 0, 100);

	GLuint goblinTexture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint batTexture = Texture2D::LoadTexture2D("SpriteSheets/rat and bat spritesheet calciumtrice.png");
	GLuint SnakeTexture = Texture2D::LoadTexture2D("SpriteSheets/snake spritesheet calciumtrice.png");
	GLuint circleTexture = Texture2D::LoadTexture2D("Images/Circle.png");
	GLuint CursorTexture = Texture2D::LoadTexture2D("Images/Cursor_Default.png");

	Transform* transform;
	GameObject* gameObject;

	PhysicsMaterial physicsMaterial = { 30, 0.8, 0.5, 10 };

	//Cursor
	transform = new Transform();
	gameObject = new GameObject("Cursor", transform, false);
	gameObject->AddComponent<Sprite>(CursorTexture, 16, 16);
	mGameObjects.emplace_back(gameObject);

	MainMenuPawn* menu = new MainMenuPawn(gameObject);

	//player 1
	transform = new Transform(Vector3D(-100, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 1", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 4, 10);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 1", { 0,0,0 }, 0, 20, CENTER);
	gameObject->AddComponent<Box2D>(48, 48, Vector2D());
	gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<AnimatorCharacter>();
	mGameObjects.emplace_back(gameObject);
	PlayerPawn* characterController = new PlayerPawn(gameObject, menu);

	//player 2
	transform = new Transform(Vector3D(100, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 2", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 4, 10);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 2", { 0,0,0 }, 0, 20, CENTER);
	gameObject->AddComponent<Box2D>(48, 48, Vector2D());
	gameObject->AddComponent<RigidBody2D>(10, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<AnimatorCharacter>();
	mGameObjects.emplace_back(gameObject);
	PlayerPawn* character2Controller = new PlayerPawn(gameObject, menu);

	transform = new Transform(Vector3D(-10, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Snake", transform);
	gameObject->AddComponent<RigidBody2D>(10, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<Circle2D>(20, Vector2D());
	gameObject->AddComponent<Sprite>(SnakeTexture, 32, 32, 10, 5);
	gameObject->AddComponent<AnimatorSnake>();
	
	mGameObjects.emplace_back(gameObject);

	//Pause Menu
	UIMenu* pauseMenu = new PauseMenu(new Transform(), characterController);
	mGameObjects.emplace_back(pauseMenu);
	MenuManager::GetInstance()->AddMenu(pauseMenu);

	//PlayerControllers
	PlayerController* playerController = new PlayerController(0, characterController);
	mPlayerControllers.push_back(playerController);

	PlayerController* playerController2 = new PlayerController(1, character2Controller);
	mPlayerControllers.push_back(playerController2);
}

GameScreenLevel1::~GameScreenLevel1()
{
	MenuManager::GetInstance()->RemoveAllMenus();
}

void GameScreenLevel1::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
	//mGameObjects[2]->GetComponent<RigidBody2D>()->AddPointForce(Vector2D(-100, 0), Vector2D(0, 0.002));

	std::vector<GameObject*> collisionObejcts;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetComponent<Collider>())
			collisionObejcts.push_back(it->get());
	}

	Collision::ResolveCollisions(Collision::DetectCollisions(collisionObejcts));
}