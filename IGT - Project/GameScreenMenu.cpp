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
#include "ButtonManager.h"

#include <iostream>
#include <utility>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	mMenuButtonsManager = new ButtonManager();
	GLuint texture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	//SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");
	GLuint backgroundTexture = Texture2D::LoadTexture2D("Images/BackGround.png");
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");
	GLuint CursorTexture = Texture2D::LoadTexture2D("Images/Cursor.png");

	mShader = new BasicShader();
	
	Transform* transform;
	GameObject* gameObject;

	transform = new Transform(Vector3D(0, 0, -1), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test", transform);

	
	gameObject = new GameObject("Backgorund", transform);
	gameObject->AddComponent<Sprite>(backgroundTexture, 480, 272);
	mGameObjects.push_back(gameObject);
	

	Button* button = new Button(ButtonTexture, { 0,25 }, { 100, 20 }, "Play");
	button->mButtonID = 0;
	button->AddObserver(mMenuButtonsManager);
	mButtons.push_back(button);

	button = new Button(ButtonTexture, { 0,0 }, { 100, 20 }, "Options");
	button->mButtonID = 1;
	button->AddObserver(mMenuButtonsManager);
	mButtons.push_back(button);

	button = new Button(ButtonTexture, { 0,-25 }, { 100, 20 }, "Quit");
	button->mButtonID = 2;
	button->AddObserver(mMenuButtonsManager);
	mButtons.push_back(button);

	transform = new Transform();
	gameObject = new GameObject("Cursor", transform);
	gameObject->AddComponent<Sprite>(CursorTexture, 16, 16);
	mGameObjects.push_back(gameObject);

	MainMenuPawn* menuController = new MainMenuPawn(mButtons, gameObject);
	PlayerController* playerController = new PlayerController(0, menuController);
	mPlayerControllers.push_back(playerController);

	transform = new Transform(Vector3D(-100, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Test", transform);

	gameObject->AddComponent<Box2D>(40, 20, Vector2D(0,0));
	gameObject->AddComponent<Sprite>(texture, 48, 48, 3, 4);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	
	mGameObjects.push_back(gameObject);
	//
	//gameObject->GetComponent<TextRender>()->UpdateText("ABXY", { 0,0,0 }, 0, 15, CENTER);
	//
	//transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));
	//
	//gameObject = new GameObject("Test2", transform);
	//gameObject->AddComponent<Sprite>(texture, 48, 48, 3, 4);
	//gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	//gameObject->AddComponent<Box2D>(transform, 40, 20, Vector2D(0, 0));
	//mGameObjects.push_back(gameObject);
	//gameObject->GetComponent<TextRender>()->UpdateText("Player2", { 0,0,0 }, 0, 15, CENTER);

	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / SCREEN_SCALE, SCREEN_HEIGHT/ SCREEN_SCALE, 0, 1000);

	
	//PlayerPawn* characterController = new PlayerPawn(mGameObjects[1]);
	//PlayerPawn* character2Controller = new PlayerPawn(mGameObjects[2]);
	//
	//PlayerController* playerController = new PlayerController(0, characterController);
	//mPlayerControllers.push_back(playerController);
	//
	//PlayerController* playerController2 = new PlayerController(1, character2Controller);
	//mPlayerControllers.push_back(playerController2);

	ObjectPool<GameObject> pool(mGameObjects[2]);
	mGameObjects.push_back(pool.AquireObject());
	//mGameObjects[3]->RemoveComponent<TextRender>();
}

GameScreenMenu::~GameScreenMenu()
{
}

void GameScreenMenu::Render()
{
	GameScreen::Render();
	for (auto gameObject : mGameObjects)
	{
		if (gameObject->GetActive())
			gameObject->Render(mShader);
	}

	for (auto button : mButtons)
	{
		button->Render(mShader);
	}
}

void GameScreenMenu::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
	mGameObjects[2]->GetTransform()->mRotation += 0.05;
	//mGameObjects[1]->GetTransform()->mRotation -= 0.05;
	//mGameObjects[0]->GetTransform()->mPosition.x += 0.1f;
	//mGameObjects[1]->GetTransform()->mPosition.x -= 0.1f;

	Collision::ResolveCollisions(Collision::DetectCollisions(mGameObjects));
}