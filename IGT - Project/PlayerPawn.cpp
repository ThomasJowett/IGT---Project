#include "PlayerPawn.h"



PlayerPawn::PlayerPawn(GameObject* character)
	:mCharacter(character)
{
}


PlayerPawn::~PlayerPawn()
{
}

void PlayerPawn::MoveUp(float scale)
{
	mCharacter->GetTransform()->mPosition.y += scale;
}

void PlayerPawn::MoveRight(float scale)
{
	mCharacter->GetTransform()->mPosition.x += scale;
}
