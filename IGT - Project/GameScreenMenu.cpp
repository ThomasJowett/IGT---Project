#include "GameScreenMenu.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Constants.h"
#include "TextRender.h"

#include <iostream>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	GLuint texture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	//SoundManager::GetInstance()->PlayMusic("Music/Diesel.ogg");

	mShader = new BasicShader();

	Transform* transform = new Transform();

	GameObject* gameObject = new GameObject("Test", transform);

	Sprite* sprite = new Sprite(texture, 48, 48, 3, 4);
	//Sprite* sprite = new Sprite(texture, 2048, 2048);
	gameObject->AddComponent(sprite);

	TextRender* text = new TextRender("Fonts/nokiafc22.ttf", 8);

	if(text->UpdateText("Hello World", SDL_Colour({ 255,255,255 }), 0, 30, CENTER))
		gameObject->AddComponent(text);

	mGameObjects.push_back(gameObject);

	camera.Initialise(Vector3D(0,0, 100), SCREEN_WIDTH/4, SCREEN_HEIGHT/4, 0, 1000);

	mGameObjects[0]->GetComponent<TextRender>()->UpdateText("It Worked?", { 255,255,10 }, 0, 30, CENTER);
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
	//mGameObjects[0]->GetTransform()->mRotation += 0.05;
	//camera.GetTransform()->mPosition.x += 0.05f;
	//mGameObjects[0]->GetTransform()->mPosition.x += 0.05f;
	//mGameObjects[0]->GetTransform()->mPosition.y += 0.05f;
}