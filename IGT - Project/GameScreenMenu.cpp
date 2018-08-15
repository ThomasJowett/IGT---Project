#include "GameScreenMenu.h"
#include "GameScreenManager.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "TextRender.h"
#include "Collider.h"
#include "Collision.h"
#include "MainMenuPawn.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "MenuManager.h"
#include "Settings.h"

#include "ObjectPool.h"

#include <iostream>
#include <utility>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	mCamera.GetTransform()->mPosition = Vector3D(0, 0, 100);

	SoundManager::GetInstance()->LoadMusic("Music/Diesel.ogg");

	GLuint texture =			Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint backgroundTexture =	Texture2D::LoadTexture2D("Images/BackGround.png");
	GLuint ButtonTexture =		Texture2D::LoadTexture2D("Images/ButtonsMenu.png");
	GLuint CursorTexture =		Texture2D::LoadTexture2D("Images/Cursor_Default.png");
	
	Transform* transform;
	GameObject* gameObject;
	
	//Background Image
	transform = new Transform(Vector3D(0, 0, -1), 0, Vector2D(1, 1));
	gameObject = new GameObject("Backgorund", transform);
	gameObject->AddComponent<Sprite>(backgroundTexture, 600, 600);
	mGameObjects.emplace_back(gameObject);

	//Main Menu
	UIMenu* mainMenu = new MainMenu(new Transform());
	mUIWidgets.emplace_back(mainMenu);
	MenuManager::GetInstance()->AddMenu(mainMenu);

	//Options Menu
	UIMenu* optionsMenu = new OptionsMenu(new Transform());
	mUIWidgets.emplace_back(optionsMenu);
	MenuManager::GetInstance()->AddMenu(optionsMenu);
	
	//Cursor
	transform = new Transform();
	gameObject = new GameObject("Cursor", transform);
	gameObject->AddComponent<Sprite>(CursorTexture, 16, 16);
	mUIWidgets.emplace_back(gameObject);
	
	//Controllers
	MainMenuPawn* menuController = new MainMenuPawn(gameObject);
	PlayerController* playerController = new PlayerController(0, menuController);
	mPlayerControllers.emplace_back(playerController);
}

GameScreenMenu::~GameScreenMenu()
{
	Texture2D::DeleteAllTextures();
	MenuManager::GetInstance()->RemoveAllMenus();
}
