#pragma once
#include "UIMenu.h"
#include "MainMenu.h"

class MainMenu;

enum OptionsMenuButtons
{
	CHANGE_RESOLUTION = 0
};

class OptionsMenu :
	public UIMenu
{
public:
	OptionsMenu(Transform* transform);
	virtual ~OptionsMenu();

	void CreateButtons()override;
	void OnNotify(ButtonEvent event, int ID)override;
	void SetMainMenuRef(UIMenu* menu) { mMainMenu = (MainMenu*)menu; }
private:

	MainMenu* mMainMenu;
};

