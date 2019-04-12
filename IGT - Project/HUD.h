#pragma once
#include "UIMenu.h"

class HUD :
	public UIMenu
{
public:
	HUD(GameObject* subject);
	virtual ~HUD() {}

	virtual void CreateWidgets()override {}

	void OnNotify(WidgetEvent event, WidgetEventData data)override {}
protected:
	GameObject* mSubject;
};

class Player1HUD :
	public HUD
{
public:
	Player1HUD(GameObject* subject);
	void CreateWidgets()override;
};

class Player2HUD:
	public HUD
{
public:
	Player2HUD(GameObject* subject);
	void CreateWidgets()override;
};