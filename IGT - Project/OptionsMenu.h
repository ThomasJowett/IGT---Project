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

	void Up()override;
	void Down()override;
	void Left()override;
	void Right()override;
};

