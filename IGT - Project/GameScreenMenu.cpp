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

#include "ObjectPool.h"
#include "ButtonManager.h"

#include <iostream>
#include <utility>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	mMenuButtonsManager = new ButtonManager();

	SoundManager::GetInstance()->LoadMusic("Music/Diesel.ogg");

	GLuint texture =			Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint backgroundTexture =	Texture2D::LoadTexture2D("Images/BackGround.png");
	GLuint ButtonTexture =		Texture2D::LoadTexture2D("Images/ButtonsMenu.png");
	GLuint CursorTexture =		Texture2D::LoadTexture2D("Images/Cursor.png");

	mShader = new BasicShader();
	
	Transform* transform;
	GameObject* gameObject;
	
	transform = new Transform(Vector3D(0, 0, -1), 0, Vector2D(1, 1));
	gameObject = new GameObject("Backgorund", transform);
	gameObject->AddComponent<Sprite>(backgroundTexture, 480, 272);
	mGameObjects.emplace_back(gameObject);
	
	std::vector<Button*> buttons;
	Button* button = new Button(ButtonTexture, { 0,25 }, { 100, 20 }, "Play");
	button->mButtonID = 0;
	button->AddObserver(mMenuButtonsManager);
	buttons.push_back(button);
	mGameObjects.emplace_back(button);
	
	button = new Button(ButtonTexture, { 0,0 }, { 100, 20 }, "Options");
	button->mButtonID = 1;
	button->AddObserver(mMenuButtonsManager);
	buttons.push_back(button);
	mGameObjects.emplace_back(button);
	
	button = new Button(ButtonTexture, { 0,-25 }, { 100, 20 }, "Quit");
	button->mButtonID = 2;
	button->AddObserver(mMenuButtonsManager);
	buttons.push_back(button);
	mGameObjects.emplace_back(button);
	
	transform = new Transform();
	gameObject = new GameObject("Cursor", transform);
	gameObject->AddComponent<Sprite>(CursorTexture, 16, 16);
	mGameObjects.emplace_back(gameObject);
	
	MainMenuPawn* menuController = new MainMenuPawn(buttons, gameObject);
	PlayerController* playerController = new PlayerController(0, menuController);
	mPlayerControllers.emplace_back(playerController);

	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / SCREEN_SCALE, SCREEN_HEIGHT/ SCREEN_SCALE, 0, 1000);
}

GameScreenMenu::~GameScreenMenu()
{
	Texture2D::DeleteAllTextures();
	delete mMenuButtonsManager;
}

void GameScreenMenu::Render()
{
	GameScreen::Render();
}

void GameScreenMenu::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
}