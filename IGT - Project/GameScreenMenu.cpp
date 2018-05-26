#include "GameScreenMenu.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Constants.h"
#include "TextRender.h"
#include "Collider.h"
#include "Collision.h"

#include <iostream>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	GLuint texture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	//SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");
	GLuint backgroundTexture = Texture2D::LoadTexture2D("Images/UVChecker.png");

	mShader = new BasicShader();

	Transform* transform = new Transform(Vector3D(0, 0, -1), 0, Vector2D(1, 1));
	GameObject* gameObject = new GameObject("Background", transform);

	Sprite* backgroundSprite = new Sprite(backgroundTexture, 512, 512);
	gameObject->AddComponent(backgroundSprite);
	mGameObjects.push_back(gameObject);

	transform = new Transform(Vector3D(-50, 0, 0), 0, Vector2D(1, 1));

	gameObject = new GameObject("Test", transform);

	//Sprite* sprite = new Sprite(texture, 48, 48, 3, 4);
	//Sprite* sprite = new Sprite(texture, 2048, 2048);
	//gameObject->AddComponent(sprite);

	TextRender* text = new TextRender("Fonts/nokiafc22.ttf", 8);

	//if(text->UpdateText("Hello World", SDL_Colour({ 255,255,255 }), 0, 0, CENTER))
		gameObject->AddComponent(text);

	Collider* collider = new Circle2D(transform, 25, { 0,0 });

	gameObject->AddComponent(collider);

	mGameObjects.push_back(gameObject);

	transform = new Transform(Vector3D(50, 0, 0), 0, Vector2D(1, 1));
	text = new TextRender("Fonts/nokiafc22.ttf", 8);
	//text->UpdateText("Not Collided", { 0,255,0 }, 0, -5, CENTER);
	collider = new Circle2D(transform, 25, Vector2D(0, 0));

	gameObject = new GameObject("Test2", transform);
	gameObject->AddComponent(text);
	gameObject->AddComponent(collider);
	mGameObjects.push_back(gameObject);

	camera.Initialise(Vector3D(0,0, 100), SCREEN_WIDTH/4, SCREEN_HEIGHT/4, 0, 1000);

	//mGameObjects[0]->GetComponent<TextRender>()->UpdateText("It Worked?", { 255,255,10 }, 0,0, CENTER);
}

GameScreenMenu::~GameScreenMenu()
{
}

void GameScreenMenu::Render()
{
	mShader->Bind();
	camera.Update(mShader);
	for (auto gameObject : mGameObjects)
	{
		gameObject->Render(mShader);
	}
}

void GameScreenMenu::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
	mGameObjects[1]->GetTransform()->mRotation += 0.05;
	mGameObjects[2]->GetTransform()->mRotation += 0.05;
	//camera.GetTransform()->mPosition.x += 0.05f;
	mGameObjects[1]->GetTransform()->mPosition.x += 0.1f;
	mGameObjects[2]->GetTransform()->mPosition.x -= 0.1f;

	Collision::ResolveCollisions(Collision::DetectCollisions(mGameObjects));
}