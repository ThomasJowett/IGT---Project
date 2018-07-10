#include "GameScreenLevel1.h"
#include "Constants.h"
#include "GameScreenManager.h"
#include "PlayerPawn.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Collider.h"
#include "Collision.h"
#include "TextRender.h"
#include "RigidBody2D.h"

GameScreenLevel1::GameScreenLevel1()
{
	mShader = new BasicShader();
	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / SCREEN_SCALE, SCREEN_HEIGHT / SCREEN_SCALE, 0, 1000);

	GLuint goblinTexture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint circleTexture = Texture2D::LoadTexture2D("Images/Circle.png");

	Transform* transform;
	GameObject* gameObject;

	//player 1
	transform = new Transform(Vector3D(-100, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 1", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 3, 4);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 1", { 0,0,0 }, 0, 20, CENTER);
	gameObject->AddComponent<Box2D>(48, 48, Vector2D());
	mGameObjects.emplace_back(gameObject);
	PlayerPawn* characterController = new PlayerPawn(gameObject);
	PlayerAnimation = new Animation(6, 3, 0.2f, gameObject->GetComponent<Sprite>());

	//player 2
	transform = new Transform(Vector3D(100, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 2", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 3, 4);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 2", { 0,0,0 }, 0, 20, CENTER);
	gameObject->AddComponent<Box2D>(48, 48, Vector2D());
	mGameObjects.emplace_back(gameObject);
	PlayerPawn* character2Controller = new PlayerPawn(gameObject);

	transform = new Transform();
	PhysicsMaterial physicsMaterial = { 3, 0.8, 0.5, 10 };
	gameObject = new GameObject("Circle", transform);
	gameObject->AddComponent<RigidBody2D>(10, Vector2D(0, 0), 10, 0.1, physicsMaterial);
	gameObject->AddComponent<Circle2D>(10, Vector2D());
	gameObject->AddComponent<Sprite>(circleTexture, 20, 20);
	mGameObjects.emplace_back(gameObject);

	transform = new Transform(Vector3D(-10, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Circle2", transform);
	gameObject->AddComponent<RigidBody2D>(10, Vector2D(0, 0), 10, 0.1, physicsMaterial);
	gameObject->AddComponent<Circle2D>(20, Vector2D());
	gameObject->AddComponent<Sprite>(circleTexture, 40, 40);
	mGameObjects.emplace_back(gameObject);

	PlayerController* playerController = new PlayerController(0, characterController);
	mPlayerControllers.push_back(playerController);

	PlayerController* playerController2 = new PlayerController(1, character2Controller);
	mPlayerControllers.push_back(playerController2);
}

GameScreenLevel1::~GameScreenLevel1()
{
}

void GameScreenLevel1::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
	mGameObjects[2]->GetComponent<RigidBody2D>()->AddPointForce(Vector2D(-100, 0), Vector2D(0, 0.002));

	std::vector<GameObject*> collisionObejcts;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetComponent<Collider>())
			collisionObejcts.push_back(it->get());
	}

	Collision::ResolveCollisions(Collision::DetectCollisions(collisionObejcts));

	PlayerAnimation->Update(deltaTime);
}