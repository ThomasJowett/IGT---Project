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
	mCamera.GetTransform()->mPosition = Vector3D(0, 0, 0);

	SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");

	GLuint texture =			Texture2D::GetTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint backgroundTexture =	Texture2D::GetTexture2D("Images/BackGround.png");
	GLuint ButtonTexture =		Texture2D::GetTexture2D("Images/ButtonsMenu.png");
	
	Transform* transform;
	GameObject* gameObject;

	//Controllers
	MainMenuPawn* menuController = new MainMenuPawn();
	PlayerController* playerController = new PlayerController(0, menuController);
	mPlayerControllers.emplace_back(playerController);
	
	//Background Image
	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));
	gameObject = new GameObject("Backgorund", transform);
	gameObject->AddComponent<Sprite>(backgroundTexture, 600, 600);
	mGameObjects.emplace_back(gameObject);
	Root->AddChild(gameObject);

	//Main Menu
	UIMenu* mainMenu = new MainMenu(new Transform());
	mUIWidgets.emplace_back(mainMenu);
	RootWidget->AddChild(mainMenu);
	MenuManager::GetInstance()->AddMenu(mainMenu);

	//Options Menu
	UIMenu* optionsMenu = new OptionsMenu(new Transform());
	mUIWidgets.emplace_back(optionsMenu);
	RootWidget->AddChild(optionsMenu);
	MenuManager::GetInstance()->AddMenu(optionsMenu);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

GameScreenMenu::~GameScreenMenu()
{
	//Texture2D::DeleteAllTextures();
	MenuManager::GetInstance()->RemoveAllMenus();
}
