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


	Transform* transform = new Transform(Vector3D(0, 0, -1), 0, Vector2D(1, 1));
	GameObject* gameObject = new GameObject("Background", transform);

	Sprite* backgroundSprite = new Sprite(backgroundTexture, 512, 512);
	//gameObject->AddComponent(backgroundSprite);
	mGameObjects.push_back(gameObject);

	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test", transform);

	//Sprite* sprite = new Sprite(texture, 10, 50, 3, 4);
	Sprite* sprite = new Sprite(texture, 55, 10);
	gameObject->AddComponent(sprite);

	TextRender* text = new TextRender("Fonts/nokiafc22.ttf", 8);

	//if(text->UpdateText("Hello World", SDL_Colour({ 255,255,255 }), 0, 0, CENTER))
	gameObject->AddComponent(text);

	Collider* collider = new Box2D(transform, 50, 10, Vector2D(0, 0));

	gameObject->AddComponent(collider);

	mGameObjects.push_back(gameObject);

	transform = new Transform(Vector3D(0, 0, 0), 0, Vector2D(1, 1));
	text = new TextRender("Fonts/nokiafc22.ttf", 8);
	//text->UpdateText("Not Collided", { 0,255,0 }, 0, -5, CENTER);
	collider = new Box2D(transform, 50, 10, Vector2D(0, 0));

	sprite = new Sprite(texture, 55, 10);

	gameObject = new GameObject("Test2", transform);
	gameObject->AddComponent(text);
	gameObject->AddComponent(collider);
	gameObject->AddComponent(sprite);
	mGameObjects.push_back(gameObject);

	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0, 1000);

	MainMenuController* menuController = new MainMenuController();

	PlayerController* playerController = new PlayerController(0, menuController);
	mPlayerControllers.push_back(playerController);

	ObjectPool<GameObject> pool(mGameObjects[1]);
	mGameObjects.push_back(pool.GetObjectA());
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
	mGameObjects[1]->GetTransform()->mRotation += 0.05;
	mGameObjects[2]->GetTransform()->mRotation -= 0.05;
	mGameObjects[1]->GetTransform()->mPosition.x += 0.1f;
	mGameObjects[2]->GetTransform()->mPosition.x -= 0.1f;

	Collision::ResolveCollisions(Collision::DetectCollisions(mGameObjects));
}