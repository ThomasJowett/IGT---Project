#pragma once
#include "UIMenu.h"

class HUD :
	public UIMenu
{
public:
	HUD(GameObject* subject);
	virtual ~HUD() {}

	void CreateWidgets()override;

	void OnNotify(WidgetEvent event, WidgetEventData data)override {}
private:
	GameObject* mSubject;
};
