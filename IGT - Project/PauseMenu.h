#pragma once

#include "UIMenu.h"

class PauseMenu
	:public UIMenu
{
public:
	PauseMenu(Transform* transform);
	void CreateButtons()override;
	void OnNotify(ButtonEvent event, int ID)override;
};