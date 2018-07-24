#pragma once
#include "UIMenu.h"
#include "OptionsMenu.h"

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
	void SetMainMenuRef(UIMenu* menu) { mOptionsMenu = (OptionsMenu*)menu; }
private:
	OptionsMenu* mOptionsMenu;
};