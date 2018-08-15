#pragma once

#include "UIMenu.h"

class PauseMenu
	:public UIMenu
{
public:
	PauseMenu(Transform* transform, iInput* playerPawn);
	void CreateWidgets()override;
	void OnNotify(WidgetEvent event, WidgetEventData data)override;

	void Back()override;
private:
	iInput * mPlayerPawn;
};