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

	void CreateButtons()override;

	void OnNotify(ButtonEvent event, int ID)override;
	
	//overrides the back function as there is no menu to go back to
	void Back()override {}
};