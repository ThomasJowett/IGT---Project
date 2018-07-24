#pragma once
#include "iInput.h"
#include "Messaging.h"
#include "UIMenu.h"
#include <vector>

class UIMenu;

class MenuManager
{
public:
	~MenuManager();

	static MenuManager * GetInstance();

	void ChangeToMenu(int menu);

	int AddMenu(UIMenu* menu);

	UIMenu* GetCurrentMenu();

private:
	MenuManager();

	std::vector<UIMenu*>mMenus;
	
	int mCurrentMenuID;
	int mPreviousMenuID;
};
