#include "GameScreenMenu.h"
#include "GameScreenManager.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Constants.h"
#include "TextRender.h"
#include "Collider.h"
#include "Collision.h"
#include "MainMenuController.h"
#include "ObjectPool.h"

#include <iostream>
#include <utility>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	GLuint texture = Texture2D::LoadTexture2D("SpriteSheets/oblinSprites.png");
	//SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");
	GLuint backgroundTexture = Texture2D::LoadTexture2D("Images/UVChecker.png");

	mShader = new BasicShader();
	
	Transform* transform;
	GameObject* gameObject;

	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test", transform);
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->AddComponent<Sprite>(texture, 55, 10);

	mGameObjects.push_back(gameObject);
	
	gameObject->GetComponent<TextRender>().UpdateText("Hello", { 0,0,0 }, 0, 0, CENTER);
	std::cout << gameObject->GetComponent<TextRender>().GetText() << std::endl;
	std::cout << gameObject->GetComponent<Sprite>().GetCurrentFrame() << std::endl;

	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test2", transform);
	gameObject->AddComponent<Sprite>(texture, 55, 10);
	mGameObjects.push_back(gameObject);

	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0, 1000);

	MainMenuController* menuController = new MainMenuController();

	PlayerController* playerController = new PlayerController(0, menuController);
	mPlayerControllers.push_back(playerController);

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
	mGameObjects[0]->GetTransform()->mRotation += 0.05;
	mGameObjects[1]->GetTransform()->mRotation -= 0.05;
	mGameObjects[0]->GetTransform()->mPosition.x += 0.1f;
	mGameObjects[1]->GetTransform()->mPosition.x -= 0.1f;

	Collision::ResolveCollisions(Collision::DetectCollisions(mGameObjects));
}