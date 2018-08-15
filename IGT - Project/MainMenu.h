#pragma once
#include "UIMenu.h"

class OptionsMenu;

enum MainMenuButtons
{
	PLAY = 0,
	OPTIONS,
	QUIT
};

class MainMenu :
	public UIMenu
{
public:
	MainMenu(Transform* transform);
	virtual ~MainMenu();

	void CreateWidgets()override;

	void OnNotify(WidgetEvent event, WidgetEventData data)override;
	
	//overrides the back function as there is no menu to go back to
	void Back()override {}
};