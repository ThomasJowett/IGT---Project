#include "PlayerPawn.h"
#include <iostream>
#include "MenuManager.h"
#include "GameScreenManager.h"

PlayerPawn::PlayerPawn(GameObject* character, iInput* menuPawn)
	:mCharacter(character),mMenuPawn(menuPawn)
{
	mRigidBody = character->GetComponent<RigidBody2D>();
	mAttack = character->GetComponent<Attack>();

	mWalkSpeed = 5000;
}


PlayerPawn::~PlayerPawn()
{
}

void PlayerPawn::MoveUp(float scale)
{ 
	mRigidBody->AddForce(Vector2D(0.0f, scale * mWalkSpeed));
}

void PlayerPawn::MoveRight(float scale)
{
	mRigidBody->AddForce(Vector2D(scale * mWalkSpeed, 0.0f));
}

void PlayerPawn::Start()
{
	MenuManager::GetInstance()->ShowCurrentMenu(true);
	GameScreenManager::GetInstance()->GetCurrentScreen()->GetPlayerControllers()[0]->PossesPawn(mMenuPawn);
}

