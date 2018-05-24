#include "GameScreenMenu.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Constants.h"

#include <iostream>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	mTextRender = new TextRender("Fonts/nokiafc22.ttf", 24);
	GLuint texture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	//SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");

	mShader = new BasicShader();

	Transform* transform = new Transform();

	GameObject* gameObject = new GameObject("Test", transform);

	Sprite* sprite = new Sprite(texture, 48, 48, 3, 4);
	//Sprite* sprite = new Sprite(texture, 2048, 2048);
	gameObject->AddRenderableComponent(sprite);

	mGameObjects.push_back(gameObject);

	camera.Initialise(Vector3D(0,0, 100), SCREEN_WIDTH/4, SCREEN_HEIGHT/4, 0, 1000);
}

GameScreenMenu::~GameScreenMenu()
{
}

void GameScreenMenu::Render()
{
	mShader->Bind();
	camera.Update(mShader);
	//mTextRender->DisplayText("Hello World", SDL_Colour{255, 255, 255}, 960, 540, CENTER);
	for (auto gameObject : mGameObjects)
	{
		gameObject->Render(mShader);
	}
}

void GameScreenMenu::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);
	//mGameObjects[0]->GetTransform()->mRotation = 3.14;
	//camera.GetTransform()->mPosition.x += 0.05f;
	//mGameObjects[0]->GetTransform()->mPosition.x += 0.05f;
	//mGameObjects[0]->GetTransform()->mPosition.y += 0.05f;
}