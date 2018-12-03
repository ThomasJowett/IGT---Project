#pragma once
#include "UIMenu.h"

class HUD :
	public UIMenu
{
public:
	HUD();
	virtual ~HUD() {}

	void CreateWidgets()override;

	void OnNotify(WidgetEvent event, WidgetEventData data)override {}
};
