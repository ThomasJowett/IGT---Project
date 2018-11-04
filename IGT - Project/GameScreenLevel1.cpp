#include "GameScreenLevel1.h"
#include "Settings.h"
#include "GameScreenManager.h"
#include "PlayerPawn.h"
#include "MainMenuPawn.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Collider.h"
#include "Collision.h"
#include "TextRender.h"
#include "RigidBody2D.h"
#include "AnimatorCharacter.h"
#include "AnimatorSnake.h"
#include "PauseMenu.h"


GameScreenLevel1::GameScreenLevel1()
{
	mShaderBasic = new BasicShader();

	mCamera.GetTransform()->mPosition = Vector3D(0, 0, 100);

	GLuint goblinTexture = Texture2D::LoadTexture2D("SpriteSheets/GoblinSprites.png");
	GLuint slimeTexture = Texture2D::LoadTexture2D("SpriteSheets/SlimeSprites.png");
	GLuint batTexture = Texture2D::LoadTexture2D("SpriteSheets/rat and bat spritesheet calciumtrice.png");
	GLuint SnakeTexture = Texture2D::LoadTexture2D("SpriteSheets/snake spritesheet calciumtrice.png");
	GLuint circleTexture = Texture2D::LoadTexture2D("Images/Circle.png");
	GLuint squareTexture = Texture2D::LoadTexture2D("Images/Square.png");
	GLuint CursorTexture = Texture2D::LoadTexture2D("Images/Cursor_Default.png");
	GLuint BarrelTexture = Texture2D::LoadTexture2D("Images/Barrel_Closed.png");


	Transform* transform;
	GameObject* gameObject;

	PhysicsMaterial physicsMaterial = { 30, 0.8, 0.5, 10 };
	PhysicsMaterial physicsMaterialcircle = { 3, 0.8, 0.5, 1 };

	//Cursor
	transform = new Transform();
	GameObject* cursor = new GameObject("Cursor", transform, false);
	cursor->AddComponent<Sprite>(CursorTexture, 16, 16);
	RootWidget->AddChild(cursor);
	mUIWidgets.emplace_back(cursor);

	MainMenuPawn* menu = new MainMenuPawn(cursor);

	//player 1
	transform = new Transform(Vector3D(191.75, -1424, 1), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 1", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 4, 10,Vector2D( 0, 16 ));
	//gameObject->AddComponent<Sprite>(squareTexture, 20, 10, Vector2D(0, -20));
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 1", { 0,0,0 }, 0, 48, CENTER);
	gameObject->AddComponent<Box2D>(20, 10, Vector2D(0, 0));
	gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<AnimatorCharacter>();
	mGameObjects.emplace_back(gameObject);
	Root->AddChild(gameObject);
	PlayerPawn* characterController = new PlayerPawn(gameObject, menu);

	//player 2
	transform = new Transform(Vector3D(256, -1424, 1), 0, Vector2D(1, 1));
	gameObject = new GameObject("Player 2", transform);
	gameObject->AddComponent<Sprite>(goblinTexture, 48, 48, 4, 10, Vector2D(0,24 ));
	gameObject->AddComponent<TextRender>("Fonts/nokiafc22.ttf", 8);
	gameObject->GetComponent<TextRender>()->UpdateText("Player 2", { 0,0,0 }, 0, 48, CENTER);
	gameObject->AddComponent<Box2D>(20, 10, Vector2D(0, 0));
	gameObject->AddComponent<RigidBody2D>(1, Vector2D(0, 0), 10, 0, physicsMaterial);
	gameObject->AddComponent<AnimatorCharacter>();
	mGameObjects.emplace_back(gameObject);
	Root->AddChild(gameObject);
	PlayerPawn* character2Controller = new PlayerPawn(gameObject, menu);

	//temporary spawning enemy
	//transform = new Transform(Vector3D(-10, 0, 0), 0, Vector2D(1, 1));
	//gameObject = new GameObject("Snake", transform);
	//gameObject->AddComponent<RigidBody2D>(10, Vector2D(0, 0), 10, 0, physicsMaterial);
	//gameObject->AddComponent<Circle2D>(8, Vector2D());
	//gameObject->AddComponent<Sprite>(SnakeTexture, 32, 32, 10, 5);
	//gameObject->AddComponent<AnimatorSnake>();
	//
	//mGameObjects.emplace_back(gameObject);

	//Pause Menu
	UIMenu* pauseMenu = new PauseMenu(new Transform(), characterController);
	mUIWidgets.emplace_back(pauseMenu);
	RootWidget->AddChild(pauseMenu);
	MenuManager::GetInstance()->AddMenu(pauseMenu);
	

	//PlayerControllers
	PlayerController* playerController = new PlayerController(0, characterController);
	mPlayerControllers.push_back(playerController);

	PlayerController* playerController2 = new PlayerController(1, character2Controller);
	mPlayerControllers.push_back(playerController2);
	
	
	//transform = new Transform(Vector3D(0, 0, 5), 0, Vector2D(1, 1));
	//gameObject = new GameObject("Circle", transform);
	//gameObject->AddComponent<Sprite>(circleTexture, 32, 32);
	//gameObject->AddComponent<Circle2D>(16, Vector2D());
	//mGameObjects.emplace_back(gameObject);
	//Root->AddChild(gameObject);
	//
	//
	//transform = new Transform(Vector3D(-80, 0, 5), 0, Vector2D(1, 1));
	//gameObject = new GameObject("Square", transform);
	//gameObject->AddComponent<Sprite>(squareTexture, 32, 32);
	//gameObject->AddComponent<Box2D>(32, 32, Vector2D());
	//mGameObjects.emplace_back(gameObject);
	//
	//Root->AddChild(gameObject);
	//
	transform = new Transform(Vector3D(0, -50, 5), 0, Vector2D(1, 1));
	gameObject = new GameObject("Ball", transform);
	gameObject->AddComponent<Sprite>(BarrelTexture, 32, 32, Vector2D(0,8));
	gameObject->AddComponent<Circle2D>(8, Vector2D());
	gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, physicsMaterialcircle);
	mGameObjects.emplace_back(gameObject);
	Root->AddChild(gameObject);
	
	for (int i = 0; i < 10; i++)
	{
		gameObject = new GameObject(*gameObject);
		gameObject->GetTransform()->mPosition = Vector3D(200 * (float)rand() / (RAND_MAX)+250, 250 * (float)rand() / (RAND_MAX)-350, 5);
		//gameObject->GetComponent<RigidBody2D>()->SetVelocity(Vector2D(80 * (float)rand() / (RAND_MAX)-40, 80 * (float)rand() / (RAND_MAX)-40));
		mGameObjects.emplace_back(gameObject);
		Root->AddChild(gameObject);
	}
	//
	//transform = new Transform(Vector3D(0, -50, 5), 0, Vector2D(1, 1));
	//gameObject = new GameObject("little box", transform);
	//gameObject->AddComponent<Sprite>(squareTexture, 16, 16);
	//gameObject->AddComponent<Box2D>(16, 16, Vector2D());
	//gameObject->AddComponent<RigidBody2D>(100, Vector2D(0, 0), 1, 0, physicsMaterialcircle);
	//mGameObjects.emplace_back(gameObject);
	//Root->AddChild(gameObject);

	mTileMap = new TileMap("TestMap.tmx");
	mGameObjects.emplace_back(mTileMap);
	Root->AddChild(mTileMap);
}

GameScreenLevel1::~GameScreenLevel1()
{
	MenuManager::GetInstance()->RemoveAllMenus();
}

void GameScreenLevel1::Update(float deltaTime, std::vector<SDL_Event> events)
{
	GameScreen::Update(deltaTime, events);

	std::vector<GameObject*> collisionObejcts;

	for (std::vector< std::unique_ptr<GameObject>> ::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		if (it->get()->GetComponent<Collider>())
		{
			collisionObejcts.push_back(it->get());
		}

		if(it->get()->GetLayer() == SORTED)
			SortObjectsDepth(it->get());
	}

	Collision::DetectCollisions(collisionObejcts);

	Collision::DetectCollisions(mTileMap, collisionObejcts);

	mCamera.GetTransform()->mPosition.x = mGameObjects[0]->GetTransform()->mPosition.x;
	mCamera.GetTransform()->mPosition.y = mGameObjects[0]->GetTransform()->mPosition.y;
}

void GameScreenLevel1::SortObjectsDepth(GameObject* gameObject)
{
	Vector3D position = gameObject->GetTransform()->mPosition;

	Camera* camera = Settings::GetInstance()->GetCamera();

	float topOfScreen = camera->GetTransform()->mPosition.y + (camera->GetOrthoHeight() / 2);
	float bottomofScreen = camera->GetTransform()->mPosition.y - (camera->GetOrthoHeight() / 2);

	if (position.y < topOfScreen && position.y > bottomofScreen)
	{
		float alpha = (position.y - camera->GetTransform()->mPosition.y + (camera->GetOrthoHeight() / 2)) / camera->GetOrthoHeight();
		float neardepth = camera->GetTransform()->mPosition.z - camera->GetNearDepth() - 1.0f;
		float fardepth =  camera->GetTransform()->mPosition.z - camera->GetFarDepth() + 1.0f;

		float objectDepth = neardepth + alpha * (fardepth - neardepth);

		gameObject->GetTransform()->mPosition.z = objectDepth;
	}
}
