#include "PlayerPawn.h"
#include <iostream>
#include "MenuManager.h"
#include "GameScreenManager.h"

PlayerPawn::PlayerPawn(GameObject* character, iInput* menuPawn)
	:mCharacter(character),mMenuPawn(menuPawn)
{
	mRigidBody = character->GetComponent<RigidBody2D>();

	mWalkSpeed = 2000;
}


PlayerPawn::~PlayerPawn()
{
}

void PlayerPawn::MoveUp(float scale)
{
	mRigidBody->AddForce(Vector2D(0.0f, scale*mWalkSpeed));
	//mCharacter->GetTransform()->mPosition.y += scale;
}

void PlayerPawn::MoveRight(float scale)
{
	mRigidBody->AddForce(Vector2D(scale*mWalkSpeed, 0.0f));
	//mCharacter->GetTransform()->mPosition.x += scale;
}

void PlayerPawn::Start()
{
	MenuManager::GetInstance()->ShowCurrentMenu(true);
	GameScreenManager::GetInstance()->GetCurrentScreen()->GetPlayerControllers()[0]->PossesPawn(mMenuPawn);
}
