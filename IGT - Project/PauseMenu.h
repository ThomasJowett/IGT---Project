#pragma once

#include "UIMenu.h"

class PauseMenu
	:public UIMenu
{
public:
	PauseMenu(Transform* transform, iInput* playerPawn);
	void CreateButtons()override;
	void OnNotify(ButtonEvent event, int ID)override;

	void Back()override;
private:
	iInput * mPlayerPawn;
};