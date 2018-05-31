#pragma once
#include "iInput.h"
#include "GameObject.h"
class PlayerPawn :
	public iInput
{
public:
	PlayerPawn(GameObject* character);
	~PlayerPawn();

	void MoveUp(float scale) override;
	void MoveRight(float scale)override;
	void LookUp(float scale) {}
	void LookRight(float scale) {}

	void Up()override {}
	void Down()override {}
	void Left()override {}
	void Right()override {}

	void Start()override {}
	void Select()override {}

	void AButton()override {}
	void BButton()override {}
	void XButton()override {}
	void YButton()override {}

	void RightTrigger(float scale) {}
	void LeftTrigger(float scale) {}

	void RightBumper()override {}
	void LeftBumper()override {}

	void MousePosition(int x, int y) {}

private:
	GameObject* mCharacter;
};

