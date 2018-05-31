#include "GameScreenMenu.h"
#include "GameScreenManager.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Constants.h"
#include "TextRender.h"
#include "Collider.h"
#include "Collision.h"
#include "MainMenuPawn.h"
#include "PlayerPawn.h"
#include "ObjectPool.h"

#include <iostream>
#include <utility>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	GLuint texture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	//SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");
	GLuint backgroundTexture = Texture2D::LoadTexture2D("Images/BackGround.png");

	mShader = new BasicShader();
	
	Transform* transform;
	GameObject* gameObject;

	transform = new Transform(Vector3D(0, 0, -1), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test", transform);

	
	gameObject = new GameObject("Backgorund", transform);
	gameObject->AddComponent<Sprite>(backgroundTexture, 480, 272);
	mGameObjects.push_back(gameObject);

	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test", transform);
	
	gameObject->AddComponent<Box2D>(transform, 40, 20, Vector2D(0,0));
	gameObject->AddComponent<Sprite>(texture, 48, 48, 3, 4);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);

	mGameObjects.push_back(gameObject);
	
	gameObject->GetComponent<TextRender>()->UpdateText("Player1", { 0,0,0 }, 0, 15, CENTER);

	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test2", transform);
	gameObject->AddComponent<Sprite>(texture, 48, 48, 3, 4);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->AddComponent<Box2D>(transform, 40, 20, Vector2D(0, 0));
	mGameObjects.push_back(gameObject);
	gameObject->GetComponent<TextRender>()->UpdateText("Player2", { 0,0,0 }, 0, 15, CENTER);

	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0, 1000);

	//MainMenuPawn* menuController = new MainMenuPawn();
	PlayerPawn* characterController = new PlayerPawn(mGameObjects[1]);
	PlayerPawn* character2Controller = new PlayerPawn(mGameObjects[2]);

	PlayerController* playerController = new PlayerController(0, characterController);
	mPlayerControllers.push_back(playerController);

	PlayerController* playerController2 = new PlayerController(1, character2Controller);
	mPlayerControllers.push_back(playerController2);

	//ObjectPool<GameObject> pool(mGameObjects[1]);
	//mGameObjects.push_back(pool.GetObjectA());
}

GameScreenMenu::~GameScreenMenu()
{
}

void GameScreenMenu::Render()
{
	GameScreen::Render();
	for (auto gameObject : mGameObjects)
	{
		gameObject->Render(mShader);
	}
}

void GameScreenMenu::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
	//mGameObjects[0]->GetTransform()->mRotation += 0.05;
	//mGameObjects[1]->GetTransform()->mRotation -= 0.05;
	//mGameObjects[0]->GetTransform()->mPosition.x += 0.1f;
	//mGameObjects[1]->GetTransform()->mPosition.x -= 0.1f;

	Collision::ResolveCollisions(Collision::DetectCollisions(mGameObjects));
}