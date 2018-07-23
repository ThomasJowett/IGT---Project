#include "PlayerPawn.h"
#include <iostream>


PlayerPawn::PlayerPawn(GameObject* character)
	:mCharacter(character)
{
	mRigidBody = character->GetComponent<RigidBody2D>();

	mWalkSpeed = 1500;
}


PlayerPawn::~PlayerPawn()
{
}

void PlayerPawn::MoveUp(float scale)
{
	mRigidBody->AddForce(Vector2D(0.0f, scale*mWalkSpeed));
	//mCharacter->GetTransform()->mPosition.y += scale;
	std::cout << mRigidBody->GetVelocity().to_string() << std::endl;
}

void PlayerPawn::MoveRight(float scale)
{
	mRigidBody->AddForce(Vector2D(scale*mWalkSpeed, 0.0f));
	//mCharacter->GetTransform()->mPosition.x += scale;
}

void PlayerPawn::Start()
{
}
